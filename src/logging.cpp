#if defined(_DEBUG) && !defined(SEUKOMOD_ENABLE_LOG)
#define SEUKOMOD_ENABLE_LOG 1
#endif

#if SEUKOMOD_ENABLE_LOG
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <ctime>
#include <sstream>

#include <windows.h>
#include <winnt.h>
#include <signal.h>

#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

typedef BOOL(WINAPI* MiniDumpWriteDump_t)(
	HANDLE hProcess,
	DWORD ProcessId,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

static volatile LONG g_dumpInProgress = 0;

static std::string CurrentTimestampForFile()
{
	std::time_t t = std::time(nullptr);
	tm tmBuf;
	localtime_s(&tmBuf, &t);
	char buf[64];
	std::strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tmBuf);
	return std::string(buf);
}

static void ComposeMinimalCrashText(std::ostringstream& ss, EXCEPTION_POINTERS* pExceptionInfo)
{
	std::string ts = CurrentTimestampForFile();
	ss << "Timestamp: " << ts << "\r\n";
	ss << "ProcessId: " << GetCurrentProcessId() << "\r\n";
	ss << "ThreadId:  " << GetCurrentThreadId() << "\r\n";

	if (pExceptionInfo && pExceptionInfo->ExceptionRecord) {
		ss << "ExceptionCode: 0x" << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::dec << "\r\n";
		ss << "ExceptionAddress: " << pExceptionInfo->ExceptionRecord->ExceptionAddress << "\r\n";

		HMODULE hModule = nullptr;
		BOOL got = GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			reinterpret_cast<LPCSTR>(pExceptionInfo->ExceptionRecord->ExceptionAddress), &hModule);
		if (got && hModule) {
			char modPath[MAX_PATH] = { 0 };
			if (GetModuleFileNameA(hModule, modPath, MAX_PATH)) {
				ss << "Module: " << modPath << "\r\n";
			}
		}
	}
	else {
		ss << "No exception pointers available.\r\n";
	}
}

static void TryWriteMiniDump(std::ostringstream& ss, EXCEPTION_POINTERS* pExceptionInfo)
{
#ifdef SEUKOMOD_DISABLE_CRASH_DUMPS
	ss << "Crash dumps are disabled in this build.\r\n";
	return;
#endif // SEUKOMOD_DISABLE_CRASH_DUMPS

	HMODULE hDbgHelp = LoadLibraryA("DbgHelp.dll");
	bool wroteDump = false;
	if (!hDbgHelp) {
		ss << "Failed to load DbgHelp.dll\r\n";
		return;
	}

	std::string ts = CurrentTimestampForFile();
	std::string dumpFileName = "seukomod_crash_" + ts + ".dmp";

	MiniDumpWriteDump_t pMiniDumpWriteDump = (MiniDumpWriteDump_t)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if (!pMiniDumpWriteDump) {
		ss << "Failed to get MiniDumpWriteDump address\r\n";
		FreeLibrary(hDbgHelp);
		return;
	}

	HANDLE hFile = CreateFileA(dumpFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) {
		ss << "CreateFile for dump failed (GetLastError=" << GetLastError() << ")\r\n";
		FreeLibrary(hDbgHelp);
		return;
	}

	MINIDUMP_EXCEPTION_INFORMATION mei;
	MINIDUMP_TYPE mdType = (MINIDUMP_TYPE)(MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithThreadInfo);
	bool ok = false;
	if (pExceptionInfo) {
		mei.ThreadId = GetCurrentThreadId();
		mei.ExceptionPointers = pExceptionInfo;
		mei.ClientPointers = FALSE;
		ok = pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdType, &mei, nullptr, nullptr);
	}
	else {
		ok = pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdType, nullptr, nullptr, nullptr);
	}

	if (ok) {
		ss << "Wrote minidump: " << dumpFileName << "\r\n";
		wroteDump = true;
	}
	else {
		ss << "MiniDumpWriteDump failed (GetLastError=" << GetLastError() << ")\r\n";
	}
	CloseHandle(hFile);
}

static void WriteMiniDumpAndLog(EXCEPTION_POINTERS* pExceptionInfo)
{
	if (InterlockedCompareExchange(&g_dumpInProgress, 1, 0) != 0) {
		// another thread is already handling the dump
		return;
	}

	std::ostringstream ss;
	ComposeMinimalCrashText(ss, pExceptionInfo);
	TryWriteMiniDump(ss, pExceptionInfo);	

	try {
		spdlog::critical("Crash detected: {}", ss.str());
	}
	catch (...) {
		// ignore
	}

	InterlockedExchange(&g_dumpInProgress, 2);
}

static LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	if (!pExceptionInfo || !pExceptionInfo->ExceptionRecord) {
		return EXCEPTION_CONTINUE_SEARCH;
	}

	DWORD code = pExceptionInfo->ExceptionRecord->ExceptionCode;

	// Only react here for serious exception codes. This avoids spurious writes for informational/debug events.
	if ((code & 0x80000000u) == 0) {
		return EXCEPTION_CONTINUE_SEARCH;
	}

	WriteMiniDumpAndLog(pExceptionInfo);
	return EXCEPTION_CONTINUE_SEARCH;
}

static LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo)
{
	WriteMiniDumpAndLog(pExceptionInfo);
	return EXCEPTION_CONTINUE_SEARCH;
}

static void MyTerminateHandler()
{
	WriteMiniDumpAndLog(nullptr);
	abort();
}

static void MyInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
	std::ostringstream ss;
	ss << "CRT invalid parameter handler called. Function: ";
	if (function) {
		char buf[512] = { 0 };
		WideCharToMultiByte(CP_UTF8, 0, function, -1, buf, sizeof(buf), nullptr, nullptr);
		ss << buf;
	}
	ss << "\r\n";
	WriteMiniDumpAndLog(nullptr);
	abort();
}

static void SignalHandler(int sig)
{
	std::ostringstream ss;
	ss << "Signal received: " << sig << "\r\n";
	WriteMiniDumpAndLog(nullptr);
	signal(sig, SIG_DFL);
	raise(sig);
}

static void InitCrashHandlers()
{
	AddVectoredExceptionHandler(1, VectoredExceptionHandler);
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	std::set_terminate(MyTerminateHandler);
	_set_invalid_parameter_handler(MyInvalidParameterHandler);

	signal(SIGABRT, SignalHandler);
	signal(SIGSEGV, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGFPE, SignalHandler);

	InterlockedExchange(&g_dumpInProgress, 0);
}

static void InitLogging()
{
	if (!AttachConsole(ATTACH_PARENT_PROCESS))
		AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);

	std::ios::sync_with_stdio(true);

	try
	{
		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("seukomod.log", true);
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		std::vector<spdlog::sink_ptr> sinks = { console_sink, file_sink };

		auto logger = std::make_shared<spdlog::logger>("seukomod", sinks.begin(), sinks.end());
		spdlog::set_default_logger(logger);

		spdlog::set_pattern("%H:%M:%S.%e [%^%l%$] %v");
		spdlog::set_level(spdlog::level::debug);
		spdlog::flush_on(spdlog::level::debug);

		spdlog::info("Debug console & file logging initialized.");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		(void)ex;
	}

	InitCrashHandlers();
}

#else // SEUKOMOD_ENABLE_LOG == 0

static void InitLogging() {}

#endif // SEUKOMOD_ENABLE_LOG

#define SEUKOMOD_ENABLE_GAME_VERSION_CHECK 1
#define SEUKOMOD_ENABLE_GAME_VERSION_CHECK_MESSAGEBOXES 1
#include <spdlog/spdlog.h>

#if SEUKOMOD_ENABLE_GAME_VERSION_CHECK
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <windows.h>
#include <optional>
#include <vector>
#pragma comment(lib, "version.lib")

struct GameVersion {
	WORD major;
	WORD minor;
	WORD build;
	WORD revision;

	constexpr GameVersion(WORD maj, WORD min, WORD bld, WORD rev)
		: major(maj), minor(min), build(bld), revision(rev) {
	}

	bool operator!=(const GameVersion& other) const {
		return major != other.major ||
			minor != other.minor ||
			build != other.build ||
			revision != other.revision;
	}

	std::string ToString() const {
		return std::to_string(major) + "." +
			std::to_string(minor) + "." +
			std::to_string(build) + "." +
			std::to_string(revision);
	}
};

static constexpr GameVersion expectedGameVersion = { 11, 44, 0, 0 };

static std::wstring GetExePath() {
	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(nullptr, exePath, MAX_PATH);
	return std::wstring(exePath);
}

static bool CheckPathManager(const std::wstring& path) {
	const std::wstring managerExe = L"manager.exe";
	if (path.length() < managerExe.length()) {
		return false;
	}

	return path.compare(path.length() - managerExe.length(), managerExe.length(), managerExe) == 0;
}

static std::optional<GameVersion> GetExeVersion(const std::wstring& exePath) {
	DWORD dummy;
	DWORD size = GetFileVersionInfoSizeW(exePath.c_str(), &dummy);
	if (size == 0) {
		return std::nullopt;
	}

	std::vector<BYTE> buffer(size);
	if (!GetFileVersionInfoW(exePath.c_str(), 0, size, buffer.data())) {
		return std::nullopt;
	}

	VS_FIXEDFILEINFO* info = nullptr;
	UINT len = 0;
	if (!VerQueryValueW(buffer.data(), L"\\", reinterpret_cast<LPVOID*>(&info), &len)) {
		return std::nullopt;
	}

	if (len == 0) {
		return std::nullopt;
	}

	WORD exeMajor = HIWORD(info->dwFileVersionMS);
	WORD exeMinor = LOWORD(info->dwFileVersionMS);
	WORD exeBuild = HIWORD(info->dwFileVersionLS);
	WORD exeRevision = LOWORD(info->dwFileVersionLS);

	return GameVersion{ exeMajor, exeMinor, exeBuild, exeRevision };
}

static void ShowVersionCheckError(const std::string& message) {
	spdlog::error(message);
#if SEUKOMOD_ENABLE_GAME_VERSION_CHECK_MESSAGEBOXES
	MessageBoxA(nullptr, message.c_str(), "Seuko mod error", MB_OK | MB_ICONERROR);
#endif // SEUKOMOD_ENABLE_GAME_VERSION_CHECK_MESSAGEBOXES
}

static bool GameVersionCheck() {
	std::wstring exePath = GetExePath();

	if (CheckPathManager(exePath)) {
		spdlog::info("Detected GTA2 Manager executable. Seuko mod will not be initialized.");
		return false;
	}

	auto versionOpt = GetExeVersion(exePath);

	if (!versionOpt.has_value()) {
		ShowVersionCheckError("Failed to get game version information. Seuko mod will not be initialized.");
		return false;
	}

	auto& version = versionOpt.value();
	spdlog::info("Game version: {}", version.ToString());

	if (version != expectedGameVersion) {
		ShowVersionCheckError("Game version mismatch. Expected version: " + expectedGameVersion.ToString() + ". Seuko mod will not be initialized.");
		return false;
	}

	return true;
}

#else // SEUKOMOD_ENABLE_GAME_VERSION_CHECK == 0

static bool GameVersionCheck()
{
	spdlog::warn("Skipping game version check");
	return true;
}

#endif // SEUKOMOD_ENABLE_GAME_VERSION_CHECK

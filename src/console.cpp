#include "common.h"

#if defined(_DEBUG)
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cstdio>
#include <iostream>
#endif

static void InitConsole()
{
#if defined(_DEBUG)
    if (!AttachConsole(ATTACH_PARENT_PROCESS))
        AllocConsole();

    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);

    std::ios::sync_with_stdio(true);

	auto logger = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::debug);

    spdlog::info("Debug console ready. DLL initializing...");
#else
	spdlog::set_level(spdlog::level::off);
#endif
}
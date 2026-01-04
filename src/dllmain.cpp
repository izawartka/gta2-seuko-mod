#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <windows.h>
#include "logging.cpp"
#include "version.h"
#include "game-version-check.cpp"
#include "core/core.h"
#include "modules/persistence/persistence.h"
#include "modules/keybinding/keybinding.h"
#include "modules/ui/ui.h"
#include "modules/modmenu/modmenu.h"

Core::Core* coreInstance = nullptr;
static bool initialized = false;

extern void UnloadOriginalDll();

static void Init()
{
	if (initialized) return;
	initialized = true;
	
	InitLogging();
	spdlog::info("Seuko mod version {} {}", SEUKOMOD_VERSION_STR, SEUKOMOD_GIT_STR);
	if (!GameVersionCheck()) return;

	coreInstance = new Core::Core();
	Core::ModuleManager* moduleManager = Core::ModuleManager::GetInstance();
	PersistenceModule::RootModule* persistenceModule = moduleManager->AddModule<PersistenceModule::RootModule>();
	KeyBindingModule::RootModule* keyBindingModule = moduleManager->AddModule<KeyBindingModule::RootModule>();
	UiModule::RootModule* uiModule = moduleManager->AddModule<UiModule::RootModule>();
	ModMenuModule::RootModule* modMenuModule = moduleManager->AddModule<ModMenuModule::RootModule>();
	ModMenuModule::MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();
	if (menuManager) menuManager->AddMenu<ModMenuModule::MainMenu>();
}

static void Deinit()
{
	if (!initialized) return;
	
	if (coreInstance != nullptr) {
		Core::ModuleManager* moduleManager = Core::ModuleManager::GetInstance();
		moduleManager->RemoveModule<ModMenuModule::RootModule>();
		moduleManager->RemoveModule<UiModule::RootModule>();
		moduleManager->RemoveModule<KeyBindingModule::RootModule>();
		moduleManager->RemoveModule<PersistenceModule::RootModule>();
		delete coreInstance;
		coreInstance = nullptr;
	}
	
	UnloadOriginalDll();
	
	initialized = false;
}

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		Init();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Deinit();
		break;
	}
	return TRUE;
}


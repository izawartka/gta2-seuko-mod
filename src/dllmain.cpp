// dllmain.cpp : Defines the entry point for the DLL application.
#include "common.h"
#include "core/core.h"
#include "modules/persistence/persistence.h"
#include "modules/ui/ui.h"
#include "modules/modmenu/modmenu.h"
#include "console.cpp"
#include "events/game-tick.h"

Core::Core* coreInstance = nullptr;

static void Init()
{
	InitConsole();
	coreInstance = new Core::Core();
	Core::ModuleManager* moduleManager = Core::ModuleManager::GetInstance();
	UiModule::RootModule* rootModule = moduleManager->AddModule<UiModule::RootModule>();
	ModMenuModule::RootModule* modMenu = moduleManager->AddModule<ModMenuModule::RootModule>();
	PersistenceModule::RootModule* persistenceModule = moduleManager->AddModule<PersistenceModule::RootModule>();
	UiModule::RootModule* uiModule = moduleManager->AddModule<UiModule::RootModule>();
	ModMenuModule::RootModule* modMenuModule = moduleManager->AddModule<ModMenuModule::RootModule>();
	ModMenuModule::MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();
	if (menuManager) menuManager->AddMenu<ModMenuModule::MainMenu>();
}

static void Deinit()
{
	Core::ModuleManager* moduleManager = Core::ModuleManager::GetInstance();
	moduleManager->RemoveModule<ModMenuModule::RootModule>();
	moduleManager->RemoveModule<UiModule::RootModule>();
	moduleManager->RemoveModule<PersistenceModule::RootModule>();
	delete coreInstance;
	coreInstance = nullptr;
}

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
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


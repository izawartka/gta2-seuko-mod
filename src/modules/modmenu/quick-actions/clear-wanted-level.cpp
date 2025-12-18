#include "clear-wanted-level.h"
#include "../cheats/set-cop-value.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_ClearWantedLevel";
static const std::wstring typeLabel = L"Clear wanted level";

ModMenuModule::ClearWantedLevelAction::ClearWantedLevelAction()
{
}

ModMenuModule::ClearWantedLevelAction::~ClearWantedLevelAction()
{
}

const std::string& ModMenuModule::ClearWantedLevelAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::ClearWantedLevelAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::ClearWantedLevelAction::Execute()
{
	auto* cheat = SetCopValueCheat::GetInstance();
	cheat->SetEnabled(true);
	cheat->SetCopValueAndDisable(0);

	ToastManager::GetInstance()->Show({ L"Wanted level cleared" });
}

const std::wstring& ModMenuModule::ClearWantedLevelAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ClearWantedLevelAction)

#include "clear-wanted-level.h"
#include "../cheats/cop-value.h"
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
	auto* cheat = CopValueCheat::GetInstance();
	if (!cheat->IsEnabled()) {
		spdlog::error("ClearWantedLevelAction::Execute: CopValueCheat is not enabled");
		ToastManager::GetInstance()->Show({ L"Failed to clear wanted level", ToastType::Error });
		return;
	}

	cheat->SetCopValue(0);
	ToastManager::GetInstance()->Show({ L"Wanted level cleared" });
}

const std::wstring& ModMenuModule::ClearWantedLevelAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ClearWantedLevelAction)

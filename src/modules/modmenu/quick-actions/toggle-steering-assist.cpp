#include "toggle-steering-assist.h"
#include "../cheats/disable-steering-assist.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_ToggleSteeringAssist";
static const std::wstring typeLabel = L"Toggle steering assist";

ModMenuModule::ToggleSteeringAssistAction::ToggleSteeringAssistAction()
{
}

ModMenuModule::ToggleSteeringAssistAction::~ToggleSteeringAssistAction()
{
}

const std::string& ModMenuModule::ToggleSteeringAssistAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::ToggleSteeringAssistAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::ToggleSteeringAssistAction::Execute()
{
	auto* cheat = DisableSteeringAssistCheat::GetInstance();
	bool doEnable = !cheat->IsEnabled();
	cheat->SetEnabled(doEnable);

	// enabled cheat = disabled assist
	if (doEnable) {
		ToastManager::GetInstance()->Show({ L"Steering assist disabled" });
	}
	else {
		ToastManager::GetInstance()->Show({ L"Steering assist enabled" });
	}
}

const std::wstring& ModMenuModule::ToggleSteeringAssistAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ToggleSteeringAssistAction)

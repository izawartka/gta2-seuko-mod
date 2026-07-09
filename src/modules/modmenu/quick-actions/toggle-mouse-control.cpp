#include "toggle-mouse-control.h"
#include "../cheats/mouse-control/mouse-control.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_ToggleMouseControl";
static const std::wstring typeLabel = L"Toggle mouse control";

ModMenuModule::ToggleMouseControlAction::ToggleMouseControlAction()
{}

ModMenuModule::ToggleMouseControlAction::~ToggleMouseControlAction()
{}

const std::string& ModMenuModule::ToggleMouseControlAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::ToggleMouseControlAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::ToggleMouseControlAction::Execute()
{
	auto* cheat = MouseControlCheat::GetInstance();
	bool doEnable = !cheat->IsEnabled();
	cheat->SetEnabled(doEnable);

	if (doEnable) {
		ToastManager::GetInstance()->Show({ L"Mouse control enabled" });
	}
	else {
		ToastManager::GetInstance()->Show({ L"Mouse control disabled" });
	}
}

const std::wstring& ModMenuModule::ToggleMouseControlAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ToggleMouseControlAction)

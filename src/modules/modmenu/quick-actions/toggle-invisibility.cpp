#include "toggle-invisibility.h"
#include "../cheats/invisibility.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_ToggleInvisibility";
static const std::wstring typeLabel = L"Toggle invisibility";

ModMenuModule::ToggleInvisibilityAction::ToggleInvisibilityAction()
{}

ModMenuModule::ToggleInvisibilityAction::~ToggleInvisibilityAction()
{}

const std::string& ModMenuModule::ToggleInvisibilityAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::ToggleInvisibilityAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::ToggleInvisibilityAction::Execute()
{
	auto* cheat = InvisibilityCheat::GetInstance();
	bool doEnable = !cheat->IsEnabled();

	if (doEnable) {
		cheat->SetEnabled(true);
		ToastManager::GetInstance()->Show({ L"Invisibility enabled" });
	}
	else {
		cheat->ResetAndDisable();
		ToastManager::GetInstance()->Show({ L"Invisibility disabled" });
	}
}

const std::wstring& ModMenuModule::ToggleInvisibilityAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ToggleInvisibilityAction)

#include "toggle-freecam.h"
#include "../cheats/camera/freecam.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_ToggleFreecam";
static const std::wstring typeLabel = L"Toggle freecam";

ModMenuModule::ToggleFreecamAction::ToggleFreecamAction() 
{
}

ModMenuModule::ToggleFreecamAction::~ToggleFreecamAction()
{
}

const std::string& ModMenuModule::ToggleFreecamAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::ToggleFreecamAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::ToggleFreecamAction::Execute()
{
	auto* cheat = FreecamCheat::GetInstance();
	bool doEnable = !cheat->IsEnabled();

	if (doEnable) {
		cheat->SetEnabled(true);
		ToastManager::GetInstance()->Show({ L"Freecam enabled" });
	}
	else {
		cheat->ResetAndDisable();
		ToastManager::GetInstance()->Show({ L"Freecam disabled" });
	}
}

const std::wstring& ModMenuModule::ToggleFreecamAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ToggleFreecamAction)

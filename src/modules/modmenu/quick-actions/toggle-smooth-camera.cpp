#include "toggle-smooth-camera.h"
#include "../cheats/camera/smooth-camera.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_ToggleSmoothCamera";
static const std::wstring typeLabel = L"Toggle smooth camera";

ModMenuModule::ToggleSmoothCameraAction::ToggleSmoothCameraAction()
{}

ModMenuModule::ToggleSmoothCameraAction::~ToggleSmoothCameraAction()
{}

const std::string& ModMenuModule::ToggleSmoothCameraAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::ToggleSmoothCameraAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::ToggleSmoothCameraAction::Execute()
{
	auto* cheat = SmoothCameraCheat::GetInstance();
	bool doEnable = !cheat->IsEnabled();
	cheat->SetEnabled(doEnable);

	if (doEnable) {
		ToastManager::GetInstance()->Show({ L"Smooth camera enabled" });
	}
	else {
		ToastManager::GetInstance()->Show({ L"Smooth camera disabled" });
	}
}

const std::wstring& ModMenuModule::ToggleSmoothCameraAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ToggleSmoothCameraAction)

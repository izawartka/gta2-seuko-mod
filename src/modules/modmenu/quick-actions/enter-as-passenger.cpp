#include "enter-as-passenger.h"
#include "../utils/enter-car-as-passenger.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_EnterAsPassenger";
static const std::wstring typeLabel = L"Enter as passenger";

ModMenuModule::EnterAsPassengerAction::EnterAsPassengerAction()
{}

ModMenuModule::EnterAsPassengerAction::~EnterAsPassengerAction()
{}

const std::string& ModMenuModule::EnterAsPassengerAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::EnterAsPassengerAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::EnterAsPassengerAction::Execute()
{
	bool success = Utils::EnterCarAsPassenger();

	if (success) {
		ToastManager::GetInstance()->Show({ L"Entering vehicle as passenger" });
	}
	else {
		ToastManager::GetInstance()->Show({ L"Failed to enter vehicle", ToastType::Warning });
	}
}

const std::wstring& ModMenuModule::EnterAsPassengerAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(EnterAsPassengerAction)

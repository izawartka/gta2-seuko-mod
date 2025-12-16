#include "get-all-weapons.h"
#include "../utils/get-all-weapons.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_GetAllWeapons";
static const std::wstring typeLabel = L"Get all weapons";

ModMenuModule::GetAllWeaponsAction::GetAllWeaponsAction()
{
}

ModMenuModule::GetAllWeaponsAction::~GetAllWeaponsAction()
{
}

const std::string& ModMenuModule::GetAllWeaponsAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::GetAllWeaponsAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::GetAllWeaponsAction::Execute()
{
	if (ModMenuModule::Utils::GetAllWeapons()) {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"All weapons given" });
	} else {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to give all weapons", ToastType::Error });
	}
}

const std::wstring& ModMenuModule::GetAllWeaponsAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(GetAllWeaponsAction)

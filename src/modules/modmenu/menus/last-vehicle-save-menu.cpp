#include "last-vehicle-save-menu.h"
#include "../root.h"
#include "../../../converters/gta-font-safe.h"
#include "../cheats/last-car.h"
#include "../cheats/saved-cars.h"
#include "../toast-manager.h"

ModMenuModule::LastVehicleSaveMenu::LastVehicleSaveMenu()
{

}

ModMenuModule::LastVehicleSaveMenu::~LastVehicleSaveMenu()
{

}

bool ModMenuModule::LastVehicleSaveMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Save Vehicle#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	// go back
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// name
	UiModule::Text* nameText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_nameController = m_menuController->CreateLatestItemController<UiModule::EditableController<std::wstring>>(
		nameText,
		L"",
		UiModule::EditableControllerOptions{ L"Name: #", L"#" }
	);
	m_nameController->SetConverter<GtaFontSafeConverter>();

	// save button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Save Vehicle", options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::LastVehicleSaveMenu::OnShow()
{
	AddEventListener<LastCarStateEvent>(&ModMenuModule::LastVehicleSaveMenu::OnLastCarStateChange);
	UpdateLastCarState();
}

void ModMenuModule::LastVehicleSaveMenu::OnHide()
{
	RemoveEventListener<LastCarStateEvent>();
}

void ModMenuModule::LastVehicleSaveMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 2: // Save Vehicle
		if (SaveCar()) {
			ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		}
		break;
	default:
		break;
	}
}

void ModMenuModule::LastVehicleSaveMenu::OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event)
{
	UpdateLastCarState();
}

void ModMenuModule::LastVehicleSaveMenu::UpdateLastCarState()
{
	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	if (!lastCarCheat->IsEnabled() || lastCarCheat->GetLastCarState() == LastCarState::NoCar) {
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
	}
}

bool ModMenuModule::LastVehicleSaveMenu::SaveCar()
{
	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	Game::Car* lastCar = lastCarCheat->GetLastCar();
	if (lastCar == nullptr) {
		ToastManager::GetInstance()->Show({ L"Last vehicle not found", ToastType::Error });
		return false;
	}
	const auto& nameOpt = m_nameController->GetValue();
	if (!nameOpt.has_value() || nameOpt.value().size() == 0) {
		ToastManager::GetInstance()->Show({ L"Please provide a name", ToastType::Error });
		return false;
	}

	SavedCarsCheat* savedCarsCheat = SavedCarsCheat::GetInstance();
	if (savedCarsCheat->GetCar(nameOpt.value()) != std::nullopt) {
		ToastManager::GetInstance()->Show({ L"Vehicle with this name already exists", ToastType::Error });
		return false;
	}

	if (savedCarsCheat->SaveCar(nameOpt.value(), lastCar)) {
		ToastManager::GetInstance()->Show({ L"Vehicle saved" });
		return true;
	}
	else {
		ToastManager::GetInstance()->Show({ L"Could not save vehicle", ToastType::Error });
		return false;
	}
}

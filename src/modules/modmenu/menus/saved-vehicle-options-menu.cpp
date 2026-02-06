#include "saved-vehicle-options-menu.h"
#include "../root.h"
#include "../cheats/saved-cars.h"

ModMenuModule::SavedVehicleOptionsMenu::SavedVehicleOptionsMenu(const std::wstring& savedCarName)
{
	m_savedCarName = savedCarName;
}

ModMenuModule::SavedVehicleOptionsMenu::~SavedVehicleOptionsMenu()
{

}

bool ModMenuModule::SavedVehicleOptionsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Saved vehicle options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	SavedCarsCheat* savedCarsCheat = SavedCarsCheat::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// name
	std::wstring carNameString = L"Name: #" + m_savedCarName + L"#";
	auto* carNameMargin = uiRoot->AddComponent<UiModule::Margin>(vertCont, options.menuControllerOptions.createdSelectableOptions.markerOffsetX, 0);
	uiRoot->AddComponent<UiModule::Text>(carNameMargin, carNameString, options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	// spawn button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Spawn Vehicle", options.textSize);

	// delete button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Delete", options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SavedVehicleOptionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	SavedCarsCheat* savedCarsCheat = SavedCarsCheat::GetInstance();
	MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();

	switch (id) {
	case 0: // Go back
		menuManager->RemoveLastMenu();
		break;
	case 1: // Spawn Vehicle
		Spawn();
		break;
	case 2: // Delete
		savedCarsCheat->DeleteCar(m_savedCarName);
		menuManager->RemoveLastMenu();
		break;
	}
}

void ModMenuModule::SavedVehicleOptionsMenu::Spawn()
{
	SavedCarsCheat* savedCarsCheat = SavedCarsCheat::GetInstance();
	bool success = savedCarsCheat->SpawnCar(m_savedCarName);

	if (!success) {
		ToastManager::GetInstance()->Show({ L"Failed to spawn " + m_savedCarName, ToastType::Error });
	}
}

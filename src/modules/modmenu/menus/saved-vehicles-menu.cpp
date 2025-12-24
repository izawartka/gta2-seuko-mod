#include "saved-vehicles-menu.h"
#include "saved-vehicle-options-menu.h"
#include "../cheats/saved-cars.h"
#include "../root.h"

ModMenuModule::SavedVehiclesMenu::SavedVehiclesMenu(size_t page)
{
	m_page = page;
}

ModMenuModule::SavedVehiclesMenu::~SavedVehiclesMenu()
{

}

bool ModMenuModule::SavedVehiclesMenu::Attach()
{
	SavedCarsCheat* savedCarsCheat = SavedCarsCheat::GetInstance();
	auto savedCarNames = savedCarsCheat->GetSavedCarsList();
	size_t pageCount = std::max((savedCarNames.size() + SAVED_CARS_PER_PAGE - 1) / SAVED_CARS_PER_PAGE, 1U);
	if (m_page >= pageCount && pageCount > 0) {
		m_page = pageCount - 1;
	}

	UiModule::Component* vertCont;
	CreateMenu(GetPageTitle(pageCount), vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	if (m_page > 0) {
		m_menuController->CreateItem<UiModule::Text>(vertCont, L"Previous page", options.textSize);
		m_prevPageItemId = m_menuController->GetLatestMenuItemId();
	}
	if (m_page < pageCount - 1) {
		m_menuController->CreateItem<UiModule::Text>(vertCont, L"Next page", options.textSize);
		m_nextPageItemId = m_menuController->GetLatestMenuItemId();
	}

	if (savedCarNames.size() > 0) uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	for (size_t i = m_page * SAVED_CARS_PER_PAGE; i < std::min(savedCarNames.size(), (m_page + 1) * SAVED_CARS_PER_PAGE); i++) {
		const std::wstring& carName = savedCarNames[i];
		m_menuController->CreateItem<UiModule::Text>(vertCont, carName, options.textSize);
		m_shownCarNames.push_back(carName);
		if (m_firstCarItemId == -1) {
			m_firstCarItemId = m_menuController->GetLatestMenuItemId();
		}
	}

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SavedVehiclesMenu::Detach()
{
	m_prevPageItemId = -1;
	m_nextPageItemId = -1;
	m_firstCarItemId = -1;
	m_shownCarNames.clear();
	DestroyMenu();
}

void ModMenuModule::SavedVehiclesMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	ModMenuModule::MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();

	switch (id) {
	case 0: // Go back
		menuManager->RemoveLastMenu();
		return;
	default:
		break;
	}

	if (id == m_prevPageItemId) { // Previous page
		MenuId thisMenuId = menuManager->GetTopMenuId();
		menuManager->AddMenu<SavedVehiclesMenu>(m_page - 1);
		menuManager->RemoveMenu(thisMenuId);
		return;
	}
	else if (id == m_nextPageItemId) { // Next page
		MenuId thisMenuId = menuManager->GetTopMenuId();
		menuManager->AddMenu<SavedVehiclesMenu>(m_page + 1);
		menuManager->RemoveMenu(thisMenuId);
		return;
	}
	else if (id >= m_firstCarItemId && m_firstCarItemId != -1) {
		size_t index = id - m_firstCarItemId;
		if (index < m_shownCarNames.size()) {
			const std::wstring& selectedCarName = m_shownCarNames[index];
			menuManager->AddMenu<SavedVehicleOptionsMenu>(selectedCarName);
		}
		return;
	}
}

std::wstring ModMenuModule::SavedVehiclesMenu::GetPageTitle(size_t pageCount) const
{
	if (pageCount > 1) {
		return L"#Saved vehicles (" + std::to_wstring(m_page + 1) + L"/" + std::to_wstring(pageCount) + L")#";
	}
	else {
		return L"#Saved vehicles";
	}
}

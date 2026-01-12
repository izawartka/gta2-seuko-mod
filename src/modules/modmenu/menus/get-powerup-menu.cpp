#include "get-powerup-menu.h"
#include "../segments/get-powerup-segment.h"
#include "../root.h"
#include "../utils/give-player-powerup.h"
#include "../toast-manager.h"

ModMenuModule::GetPowerupMenu::GetPowerupMenu()
{
	m_getPowerupSegment = CreateSegment<ModMenuModule::GetPowerupSegment>("ModMenu_GetPowerupMenu_GetPowerupSegment");
}

ModMenuModule::GetPowerupMenu::~GetPowerupMenu()
{

}

bool ModMenuModule::GetPowerupMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Get power-up#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// power-up segment
	AttachSegment(m_getPowerupSegment, this, vertCont);

	// get weapon button
	auto* getPowerupText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Get power-up", options.textSize);
	auto* getPowerupBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(getPowerupText);
	getPowerupBtn->SetCallback(this, &GetPowerupMenu::GetPowerup);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::GetPowerupMenu::Detach()
{
	DetachSegment(m_getPowerupSegment);
	DestroyMenu();
}

void ModMenuModule::GetPowerupMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::GetPowerupMenu::GetPowerup()
{
	const auto& segmentData = m_getPowerupSegment->GetSegmentData();
	if (!segmentData.has_value()) return;

	Game::POWERUP_TYPE selectedPowerup = segmentData.value().powerupType;
	if (!Utils::GivePlayerPowerup(selectedPowerup)) {
		ToastManager::GetInstance()->Show({ L"Failed to give power-up", ToastType::Error });
	}
}

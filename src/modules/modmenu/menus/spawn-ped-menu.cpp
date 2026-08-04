#include "spawn-ped-menu.h"
#include "../root.h"
#include "../segments/position-rotation-menu-segment.h"
#include "../segments/spawn-ped-segment.h"

ModMenuModule::SpawnPedMenu::SpawnPedMenu()
{
	m_posRotMenuSegment = CreateSegment<PositionRotationMenuSegment>("ModMenu_SpawnPedMenu_PositionRotationMenuSegment");
	m_SpawnPedSegment = CreateSegment<SpawnPedSegment>("ModMenu_SpawnPedMenu_SpawnSegment");
}

ModMenuModule::SpawnPedMenu::~SpawnPedMenu()
{

}

bool ModMenuModule::SpawnPedMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Spawn character#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	AttachSegment(m_posRotMenuSegment, this, vertCont);
	AttachSegment(m_SpawnPedSegment, this, vertCont);

	// spawn button
	auto spawnText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Spawn", options.textSize);
	auto spawnBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(spawnText);
	spawnBtn->SetCallback(this, &SpawnPedMenu::Spawn);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SpawnPedMenu::Detach()
{
	SaveCurrentSelectedIndex();
	DetachSegment(m_posRotMenuSegment);
	DetachSegment(m_SpawnPedSegment);
	DestroyMenu();
}

void ModMenuModule::SpawnPedMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::SpawnPedMenu::OnHide()
{
	SetSegmentsVisible(false);
}

void ModMenuModule::SpawnPedMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == 0) {
		MenuManager::GetInstance()->RemoveLastMenu();
		return;
	}

	if (m_posRotMenuSegment->OnPassedMenuAction(item, id)) {
		return;
	}

	m_SpawnPedSegment->OnPassedMenuAction(item, id);
	return;
}

void ModMenuModule::SpawnPedMenu::Spawn()
{
	auto spawnSegmentDataOpt = m_SpawnPedSegment->GetSegmentData();
	if (!spawnSegmentDataOpt.has_value()) {
		spdlog::error("Cannot spawn ped: failed to get SpawnPedSegment data.");
		return;
	}

	auto positionSegmentDataOpt = m_posRotMenuSegment->GetSegmentData();
	if (!positionSegmentDataOpt.has_value()) {
		spdlog::error("Cannot spawn ped: failed to get PositionRotationMenuSegment data");
		return;
	}

	auto& spawnSegmentData = spawnSegmentDataOpt.value();
	auto& positionSegmentData = positionSegmentDataOpt.value();

	bool success = spawnSegmentData.inNearestCar ?
		Utils::SpawnPed::SpawnPedInNearestCar(positionSegmentData.position, spawnSegmentData.pedTemplate.data) :
		Utils::SpawnPed::SpawnPed(positionSegmentData.position, positionSegmentData.rotation, spawnSegmentData.pedTemplate.data);

	if (!success) {
		ToastManager::GetInstance()->Show({ L"Failed to spawn character", ToastType::Error });
	}
}

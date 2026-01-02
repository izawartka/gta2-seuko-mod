#include "spawn-object-menu.h"
#include "../segments/position-rotation-segment.h"
#include "../segments/spawn-object-segment.h"
#include "../root.h"
#include "../utils/spawn-object.h"

ModMenuModule::SpawnObjectMenu::SpawnObjectMenu()
{
	m_posRotSegment = CreateSegment<ModMenuModule::PositionRotationSegment>("ModMenu_SpawnObjectMenu_PosRotSegment");
	m_spawnObjectSegment = CreateSegment<ModMenuModule::SpawnObjectSegment>("ModMenu_SpawnObjectMenu_SpawnSegment");
}

ModMenuModule::SpawnObjectMenu::~SpawnObjectMenu()
{

}

bool ModMenuModule::SpawnObjectMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Spawn object#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	AttachSegment(m_posRotSegment, this, vertCont);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	AttachSegment(m_spawnObjectSegment, this, vertCont);

	// spawn button
	auto spawnText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Spawn", options.textSize);
	auto spawnBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(spawnText);
	spawnBtn->SetCallback(this, &SpawnObjectMenu::Spawn);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SpawnObjectMenu::Detach()
{
	DetachSegment(m_posRotSegment);
	DetachSegment(m_spawnObjectSegment);
	DestroyMenu();
}

void ModMenuModule::SpawnObjectMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::SpawnObjectMenu::OnHide()
{
	SetSegmentsVisible(false);
}

void ModMenuModule::SpawnObjectMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::SpawnObjectMenu::Spawn()
{
	auto positionSegmentDataOpt = m_posRotSegment->GetSegmentData();
	if (!positionSegmentDataOpt.has_value()) {
		spdlog::error("Cannot spawn object: failed to get position rotation segment data.");
		return;
	}

	auto& posRotSegmentData = positionSegmentDataOpt.value();

	auto spawnObjectSegmentDataOpt = m_spawnObjectSegment->GetSegmentData();
	if (!spawnObjectSegmentDataOpt.has_value()) {
		spdlog::error("Cannot spawn object: failed to get spawn object segment data.");
		return;
	}

	auto& spawnObjectSegmentData = spawnObjectSegmentDataOpt.value();

	ModMenuModule::Utils::SpawnObject(
		posRotSegmentData.position,
		posRotSegmentData.rotation,
		spawnObjectSegmentData.objectType
	);
}

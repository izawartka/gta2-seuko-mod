#include "spawn-object-menu.h"
#include "../segments/position-rotation-menu-segment.h"
#include "../segments/spawn-object-segment.h"
#include "../root.h"
#include "../utils/spawn-object.h"
#include "../../../converters/object-type.h"

ModMenuModule::SpawnObjectMenu::SpawnObjectMenu()
{
	m_posRotMenuSegment = CreateSegment<ModMenuModule::PositionRotationMenuSegment>("ModMenu_SpawnObjectMenu_PositionMenuSegment");
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

	AttachSegment(m_posRotMenuSegment, this, vertCont);
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
	SaveCurrentSelectedIndex();
	DetachSegment(m_posRotMenuSegment);
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
		m_posRotMenuSegment->OnPassedMenuAction(item, id);
		break;
	}
}

void ModMenuModule::SpawnObjectMenu::Spawn()
{
	auto positionSegmentDataOpt = m_posRotMenuSegment->GetSegmentData();
	if (!positionSegmentDataOpt.has_value()) {
		spdlog::error("Cannot spawn object: failed to get PositionRotationMenuSegment data.");
		return;
	}

	auto& posRotSegmentData = positionSegmentDataOpt.value();

	auto spawnObjectSegmentDataOpt = m_spawnObjectSegment->GetSegmentData();
	if (!spawnObjectSegmentDataOpt.has_value()) {
		spdlog::error("Cannot spawn object: failed to get SpawnObjectSegment data.");
		return;
	}

	auto& spawnObjectSegmentData = spawnObjectSegmentDataOpt.value();

	bool success = ModMenuModule::Utils::SpawnObject(
		posRotSegmentData.position,
		posRotSegmentData.rotation,
		spawnObjectSegmentData.objectType
	);

	if (!success) {
		std::wstring objectTypeStr = ObjectTypeConverter::ConvertToString(spawnObjectSegmentData.objectType);
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to spawn " + objectTypeStr, ToastType::Error });
	}
}

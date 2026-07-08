#include "spawn-vehicle-menu.h"
#include "../segments/position-menu-segment.h"
#include "../segments/spawn-vehicle-segment.h"
#include "../../../converters/car-model-name.h"
#include "../root.h"
#include "../utils/spawn-car.h"

ModMenuModule::SpawnVehicleMenu::SpawnVehicleMenu()
{
	m_positionMenuSegment = CreateSegment<PositionMenuSegment>("ModMenu_SpawnVehicleMenu_PositionMenuSegment");
	m_spawnVehicleSegment = CreateSegment<SpawnVehicleSegment>("ModMenu_SpawnVehicleMenu_SpawnSegment");
}

ModMenuModule::SpawnVehicleMenu::~SpawnVehicleMenu()
{

}

bool ModMenuModule::SpawnVehicleMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Spawn vehicle#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	AttachSegment(m_positionMenuSegment, this, vertCont);
	AttachSegment(m_spawnVehicleSegment, this, vertCont);

	// spawn button
	auto spawnText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Spawn", options.textSize);
	auto spawnBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(spawnText);
	spawnBtn->SetCallback(this, &SpawnVehicleMenu::Spawn);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SpawnVehicleMenu::Detach()
{
	DetachSegment(m_positionMenuSegment);
	DetachSegment(m_spawnVehicleSegment);
	DestroyMenu();
}

void ModMenuModule::SpawnVehicleMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::SpawnVehicleMenu::OnHide()
{
	SetSegmentsVisible(false);
}

void ModMenuModule::SpawnVehicleMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		m_positionMenuSegment->OnPassedMenuAction(item, id);
		break;
	}
}

void ModMenuModule::SpawnVehicleMenu::Spawn()
{
	auto spawnSegmentDataOpt = m_spawnVehicleSegment->GetSegmentData();
	if (!spawnSegmentDataOpt.has_value()) {
		spdlog::error("Cannot spawn vehicle: failed to get SpawnVehicleSegment data.");
		return;
	}

	auto positionSegmentDataOpt = m_positionMenuSegment->GetSegmentData();
	if (!positionSegmentDataOpt.has_value()) {
		spdlog::error("Cannot spawn vehicle: failed to get PositionMenuSegment data");
		return;
	}

	auto& spawnSegmentData = spawnSegmentDataOpt.value();
	auto& positionSegmentData = positionSegmentDataOpt.value();

	bool success = ModMenuModule::Utils::SpawnCar(
		positionSegmentData.position,
		positionSegmentData.rotation,
		spawnSegmentData.model,
		spawnSegmentData.remap,
		spawnSegmentData.palette
	);

	if (!success) {
		std::wstring modelStr = CarModelNameConverter::ConvertToString(spawnSegmentData.model);
		ToastManager::GetInstance()->Show({ L"Failed to spawn " + modelStr, ToastType::Error });
	}
}

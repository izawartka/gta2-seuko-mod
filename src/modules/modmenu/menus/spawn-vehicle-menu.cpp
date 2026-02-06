#include "spawn-vehicle-menu.h"
#include "../segments/spawn-vehicle-segment.h"
#include "../../../converters/car-model.h"
#include "../../../converters/yes-no.h"
#include "../../../converters/car-remap.h"
#include "../root.h"
#include "../utils/spawn-car-at-player.h"

ModMenuModule::SpawnVehicleMenu::SpawnVehicleMenu()
{
	m_spawnVehicleSegment = CreateSegment<ModMenuModule::SpawnVehicleSegment>("ModMenu_SpawnVehicleMenu_SpawnSegment");
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
	DetachSegment(m_spawnVehicleSegment);
	DestroyMenu();
}

void ModMenuModule::SpawnVehicleMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::SpawnVehicleMenu::Spawn()
{
	auto segmentDataOpt = m_spawnVehicleSegment->GetSegmentData();
	if (!segmentDataOpt.has_value()) {
		spdlog::error("Cannot spawn vehicle: failed to get segment data.");
		return;
	}

	auto& segmentData = segmentDataOpt.value();

	bool success = ModMenuModule::Utils::SpawnCarAtPlayer(
		segmentData.model,
		segmentData.remap,
		segmentData.palette
	);

	if (!success) {
		std::wstring modelStr = CarModelConverter::ConvertToString(segmentData.model);
		ToastManager::GetInstance()->Show({ L"Failed to spawn " + modelStr, ToastType::Error });
	}
}

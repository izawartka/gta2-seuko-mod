#include "add-default-actions.h"
#include "../quick-action-manager.h"
#include "../quick-actions/quick-actions.h"

bool ModMenuModule::Utils::AddDefaultActions(uint16_t prevVersion)
{
	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();

	switch (prevVersion) {
	case 0: {
		spdlog::debug("Adding default actions version 0->1");

		// Save game
		QuickActionTypeIndex saveGameTypeIndex = typeid(SaveGameAction);
		quickActionManager->Add({ Game::KeyCode::DIK_Q, false, false, true }, saveGameTypeIndex);

		// Get all weapons
		QuickActionTypeIndex getAllWeaponsTypeIndex = typeid(GetAllWeaponsAction);
		quickActionManager->Add({ Game::KeyCode::DIK_W, false, false, true }, getAllWeaponsTypeIndex);

		// Spawn TANK
		QuickActionTypeIndex spawnVehicleTypeIndex = typeid(SpawnVehicleAction);
		QuickActionId spawnTankActionId = quickActionManager->Add({Game::KeyCode::DIK_T, false, false, true}, spawnVehicleTypeIndex);
		quickActionManager->SetData<SpawnVehicleAction>(spawnTankActionId, SpawnVehicleSegmentData{ Game::CAR_MODEL4_TANK, 0, Game::PALETTE_BASE_SPRITE });

		// Spawn GT24640
		QuickActionId spawnGT24640ActionId = quickActionManager->Add({ Game::KeyCode::DIK_G, false, false, true }, spawnVehicleTypeIndex);
		quickActionManager->SetData<SpawnVehicleAction>(spawnGT24640ActionId, SpawnVehicleSegmentData{ Game::CAR_MODEL4_GT24640, 2, Game::PALETTE_BASE_CAR_REMAP });

		// Clear wanted level
		QuickActionTypeIndex clearWantedLevelTypeIndex = typeid(ClearWantedLevelAction);
		QuickActionId clearWantedLevelActionId = quickActionManager->Add({ Game::KeyCode::DIK_P, false, false, true }, clearWantedLevelTypeIndex);
	}
	[[fallthrough]];
	default:
		break;
	}

	return true;
}

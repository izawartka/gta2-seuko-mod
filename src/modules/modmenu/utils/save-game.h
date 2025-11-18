#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	static void SaveGame()
	{
		Game::Ped* playerPed = Game::Memory::GetPlayerPed();
		if (!playerPed) {
			spdlog::warn("Cannot save: Player ped not found");
			return;
		}

		Game::S15_Script* s15 = Game::Memory::GetS15();
		if (s15 == nullptr) {
			spdlog::warn("Cannot save: S15 script not found");
			return;
		}

		bool* missionFlagPtr = (bool*)s15->missionPtrMaybe;
		if (missionFlagPtr == nullptr) {
			spdlog::warn("Cannot save: Mission flag pointer not found");
			return;
		}

		if (*missionFlagPtr != 0) {
			spdlog::warn("Cannot save during a mission");
			return;
		}

		playerPed->y -= Game::Utils::FromFloat(1.0f);

		Game::Menu* menu = Game::Memory::GetMenu();
		Game::Functions::SaveGame(s15, 0, menu->saveFile);

		playerPed->y += Game::Utils::FromFloat(1.0f);
		spdlog::info("Game saved");
	}
}

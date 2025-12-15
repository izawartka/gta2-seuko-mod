#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	static void SpawnCarAtPlayer(Game::CAR_MODEL4 model, short remap, Game::PALETTE_BASE palette) {
		Game::Ped* ped = Game::Memory::GetPlayerPed();
		if (!ped || !ped->gameObject || !ped->gameObject->sprite) {
			spdlog::warn("Cannot spawn vehicle: Player ped or sprite is invalid.");
			return;
		}

		if (ped->currentCar) {
			spdlog::warn("Cannot spawn vehicle: Player is already in a vehicle.");
			return;
		}

		Game::Car* car = Game::Functions::SpawnCar(
			ped->gameObject->sprite->position.x,
			ped->gameObject->sprite->position.y,
			ped->gameObject->sprite->position.z,
			ped->gameObject->spriteRotation,
			model
		);

		if (!car || !car->sprite) {
			spdlog::error("Cannot spawn vehicle: Failed to spawn vehicle model #{}.", static_cast<uint32_t>(model));
			return;
		}

		car->sprite->carColor = remap;
		car->sprite->paletteBase = palette;

		spdlog::info("Spawned vehicle model #{}.", static_cast<uint32_t>(model));

		/// TODO: Move player into the car
	}
}

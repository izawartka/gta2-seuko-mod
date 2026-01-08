#pragma once
#include "../common.h"

namespace ModMenuModule::Utils::FixCar {
	void FixCarVisualDamage(Game::Car* car);
	void FixCarEngineDamage(Game::Car* car);
	void ExtinguishCar(Game::Car* car);
}

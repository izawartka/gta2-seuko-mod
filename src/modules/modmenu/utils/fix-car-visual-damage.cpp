#include "fix-car-visual-damage.h"

void ModMenuModule::Utils::FixCarVisualDamage(Game::Car* car)
{
	Game::CAR_DELTAS_BITSTATE deltasBitmask = static_cast<Game::CAR_DELTAS_BITSTATE>(
		Game::CAR_DELTAS_BITSTATE_LIGHT_BROKEN_RR |
		Game::CAR_DELTAS_BITSTATE_LIGHT_BROKEN_RL |
		Game::CAR_DELTAS_BITSTATE_LIGHT_BROKEN_FL |
		Game::CAR_DELTAS_BITSTATE_LIGHT_BROKEN_FR |
		Game::CAR_DELTAS_BITSTATE_FRONT_GLASS_BROKEN
	);

	car->deltasBitstate = static_cast<Game::CAR_DELTAS_BITSTATE>(
		car->deltasBitstate & ~deltasBitmask
	);

	if (car->lightsBitstate & Game::CAR_LIGHTS_BITSTATE_LIGHTS_ON)
	{
		Game::Functions::TurnOnCarLights(car);
	}

	if (car->lightsBitstate & Game::CAR_LIGHTS_BITSTATE_BRAKE_LIGHTS_ON)
	{
		Game::Functions::TurnOnCarBrakeLights(car);
	}

	spdlog::info("Fixed car visual damage.");
}

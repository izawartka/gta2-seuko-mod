#include "fix-car-utils.h"

void ModMenuModule::Utils::FixCar::FixCarVisualDamage(Game::Car* car)
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

void ModMenuModule::Utils::FixCar::FixCarEngineDamage(Game::Car* car)
{
	car->carDamage = 0;

	if (car->engineState == Game::CAR_ENGINE_STATE_ON)
	{
		car->lightsBitstate = static_cast<Game::CAR_LIGHTS_BITSTATE>(car->lightsBitstate | Game::CAR_LIGHTS_BITSTATE_LIGHTS_ON);
	}

	if (car->lightsBitstate & Game::CAR_LIGHTS_BITSTATE_LIGHTS_ON)
	{
		Game::Functions::TurnOnCarLights(car);
	}

	if (car->lightsBitstate & Game::CAR_LIGHTS_BITSTATE_BRAKE_LIGHTS_ON)
	{
		Game::Functions::TurnOnCarBrakeLights(car);
	}

	Game::Functions::FixCarBrokenEngine(car);

	spdlog::info("Fixed car engine damage.");
}

void ModMenuModule::Utils::FixCar::ExtinguishCar(Game::Car* car)
{
	car->fireState = 0;
	Game::Functions::ExtinguishCar(car);

	spdlog::info("Extinguished car fire.");
}

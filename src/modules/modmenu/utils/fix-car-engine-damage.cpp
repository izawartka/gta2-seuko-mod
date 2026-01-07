#include "fix-car-engine-damage.h"

void ModMenuModule::Utils::FixCarEngineDamage(Game::Car* car)
{
	car->carDamage = 0;
	car->fireState = 0;

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

	Game::Functions::ExtinguishCar(car);
	Game::Functions::FixCarBrokenEngine(car);

	spdlog::info("Fixed car engine damage.");
}

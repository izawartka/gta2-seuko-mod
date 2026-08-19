#include "enter-car-as-passenger.h"

Game::Car* GetCarToEnterAsPassenger(Game::Ped* ped)
{
	if (!ped || !ped->gameObject || !ped->gameObject->sprite) return nullptr;

	Game::TrafficManager* trafficManager = Game::Memory::GetTrafficManager();
	if (!trafficManager) return nullptr;

	// logic below is based on 0x004a76d0
	if (ped->state == Game::PED_STATE_WASTED) return nullptr;
	if (ped->currentCar) return nullptr;
	if (ped->objective == Game::PED_OBJECTIVE_LEAVE_CAR) return nullptr;
	return Game::Functions::GetNearestCarForEnter(
		trafficManager, 
		0, 
		ped->gameObject->sprite, 3
	);
}

static Game::byte GetDoorToEnterAsPassenger(Game::Car* car)
{
	Game::byte doorIndex = 1;
	Game::Passenger* passenger = car->lastPassenger;
	while (passenger) {
		if (passenger->ped && passenger->ped->targetCarDoor == doorIndex) {
			passenger = car->lastPassenger;
			doorIndex++;
		}
		else {
			passenger = passenger->prev;
		}
	}

	return doorIndex;
}

bool ModMenuModule::Utils::EnterCarAsPassenger()
{
	Game::Ped* playerPed = Game::Utils::GetPlayerCurrentPed();
	Game::Car* targetCar = GetCarToEnterAsPassenger(playerPed);
	if (!targetCar) return false;

	// logic below is based on 0x004a76d0
	bool isTrain = Game::Functions::IsCarATrain(targetCar);
	Game::PED_OBJECTIVE newObjective = isTrain ?
		Game::PED_OBJECTIVE_ENTER_TRAIN :
		Game::PED_OBJECTIVE_ENTER_CAR;

	Game::Functions::UpdatePedStatesFromObjective(playerPed, 0, Game::PED_OBJECTIVE_NO_OBJ, 9999);
	Game::Functions::PedSetObjective(playerPed, 0, newObjective, 9999);
	playerPed->objectiveTargetCar = targetCar;
	playerPed->enterCarAsPassenger = 1;
	playerPed->targetCarDoor = GetDoorToEnterAsPassenger(targetCar);

	return true;
}

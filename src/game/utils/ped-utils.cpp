#include "ped-utils.h"
#include "../functions.h"
#include "../memory.h"

Game::Ped* Game::Utils::CreatePedInCar(Car* car)
{
	if (!car) return nullptr;

	Ped* ped = Functions::CreatePed();
	if (!ped) return nullptr;

	Functions::PutPedInCar(ped, 0, car);
	S17_Mission* mission = Memory::GetS17Mission();
	if (!mission) {
		return nullptr;
	}

	Functions::CreatePed2(mission, 0, ped);
	Functions::MakeCarDriveable1(car, 0, 5);
	Functions::MakeCarDriveable2(car, 0);
	Functions::MakeCarDriveable3(car->notEngineStruct, 0, car);
	Functions::MakeCarDriveable4(car, 0);

	return ped;
}

#pragma once
#include <string>
#include "../game/game.h"

class PedOccupationConverter {
public:
	static std::wstring ConvertToString(Game::PED_OCCUPATION value) {
		switch (value) {
		case Game::PED_OCCUPATION_PLAYER: return L"Player";
		case Game::PED_OCCUPATION_EMPTY: return L"Empty";
		// ...
		case Game::PED_OCCUPATION_DUMMY: return L"Dummy";
		case Game::PED_OCCUPATION_MISSION_DRIVER: return L"Mission Driver";
		case Game::PED_OCCUPATION_DRIVER: return L"Driver";
		// ...
		case Game::PED_OCCUPATION_BUS_PASSENGER: return L"Bus Passenger";
		case Game::PED_OCCUPATION_TRAIN_PASSENGER: return L"Train Passenger";
		case Game::PED_OCCUPATION_DRIVER2: return L"Driver 2";
		// ...
		case Game::PED_OCCUPATION_ANGRY_DRIVER: return L"Angry Driver";
		case Game::PED_OCCUPATION_PSYCHO: return L"Psycho";
		case Game::PED_OCCUPATION_MUGGER: return L"Mugger";
		case Game::PED_OCCUPATION_CARTHIEF: return L"Car Thief";
		case Game::PED_OCCUPATION_BANK_ROBBER: return L"Bank Robber";
		case Game::PED_OCCUPATION_CRIMINAL: return L"Criminal";
		case Game::PED_OCCUPATION_GANG_MEMBER: return L"Gang Member";
		// ...
		case Game::PED_OCCUPATION_ELVIS: return L"Elvis";
		case Game::PED_OCCUPATION_MEDIC: return L"Medic";
		case Game::PED_OCCUPATION_POLICE: return L"Police";
		case Game::PED_OCCUPATION_SWAT: return L"SWAT";
		case Game::PED_OCCUPATION_FBI: return L"FBI";
		case Game::PED_OCCUPATION_ARMY_IN_JEEP: return L"Army in Jeep";
		case Game::PED_OCCUPATION_GUARD: return L"Guard";
		case Game::PED_OCCUPATION_POLICE_ON_FOOT: return L"Police on Foot";
		// ...
		case Game::PED_OCCUPATION_ARMY_ON_FOOT: return L"Army on Foot";
		case Game::PED_OCCUPATION_GUARD_AGAINST_PLAYER: return L"Guard Against Player";
		case Game::PED_OCCUPATION_CRIMINAL_TYPE1: return L"Criminal Type 1";
		case Game::PED_OCCUPATION_CRIMINAL_TYPE2: return L"Criminal Type 2";
		case Game::PED_OCCUPATION_SPECIAL_GROUP_MEMBER: return L"Special Group Member";
		case Game::PED_OCCUPATION_TANK_DRIVER: return L"Tank Driver";
		// ...
		case Game::PED_OCCUPATION_ROAD_BLOCK_FBI: return L"Road Block FBI";
		case Game::PED_OCCUPATION_FIREMAN: return L"Fireman";
		case Game::PED_OCCUPATION_ROAD_BLOCK_TANK_MAN: return L"Road Block Tank Man";
		// ...
		case Game::PED_OCCUPATION_DRONE: return L"Drone";
		case Game::PED_OCCUPATION_DRIVER_GET_OUTTA_MY_CAR: return L"Driver (Get outta my car)";
		case Game::PED_OCCUPATION_STAND_STILL_BLOKE: return L"Stand Still Bloke";
		case Game::PED_OCCUPATION_ELVIS_LEADER: return L"Elvis Leader";
		case Game::PED_OCCUPATION_REFUGEES: return L"Refugees";
		case Game::PED_OCCUPATION_ANY_LAW_ENFORCEMENT: return L"Any Law Enforcement";
		case Game::PED_OCCUPATION_ANY_EMERGENCY_SERVICE_MAN: return L"Any Emergency Service Man";
		case Game::PED_OCCUPATION_ANY_GANG_MEMBER: return L"Any Gang Member";
		case Game::PED_OCCUPATION_ANY_ELVIS: return L"Any Elvis";
		case Game::PED_OCCUPATION_DRIVER3: return L"Driver3";
		case Game::PED_OCCUPATION_NO_OCCUPATION: return L"No Occupation";

		default:
			return L"Unknown " + std::to_wstring(static_cast<int32_t>(value));
		}
	}

	static bool AreEqual(Game::PED_OCCUPATION a, Game::PED_OCCUPATION b) {
		return a == b;
	}

protected:
	PedOccupationConverter() = delete;
	~PedOccupationConverter() = delete;
	PedOccupationConverter(const PedOccupationConverter&) = delete;
	PedOccupationConverter& operator=(const PedOccupationConverter&) = delete;
};

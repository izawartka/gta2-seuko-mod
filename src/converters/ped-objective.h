#pragma once
#include <string>
#include "../game/game.h"

class PedObjectiveConverter {
public:
	static std::wstring ConvertToString(Game::PED_OBJECTIVE value) {
		switch (value) {
		case Game::PED_OBJECTIVE_NO_OBJ: return L"No objective";
		case Game::PED_OBJECTIVE_FLEE_ON_FOOT_TILL_SAFE: return L"Flee on foot till safe";
		case Game::PED_OBJECTIVE_FLEE_CHAR_ON_FOOT_TILL_SAFE: return L"Flee character on foot till safe";
		case Game::PED_OBJECTIVE_FLEE_CHAR_ON_FOOT_ALWAYS: return L"Flee character on foot always";
		case Game::PED_OBJECTIVE_FLEE_CHAR_ANY_MEANS_TILL_SAFE: return L"Flee character any means till safe";
		case Game::PED_OBJECTIVE_FLEE_CHAR_ANY_MEANS_ALWAYS: return L"Flee character any means always";
		case Game::PED_OBJECTIVE_FLEE_CHAR_ALWAYS_ONCE_CAR_STOPPED: return L"Flee character any means always once car stopped";
		// ...
		case Game::PED_OBJECTIVE_GO_TO_AREA_ON_FOOT: return L"Go to area on foot";
		case Game::PED_OBJECTIVE_GO_TO_AREA_ANY_MEANS: return L"Go to area any means";
		case Game::PED_OBJECTIVE_GO_TO_AREA_IN_CAR: return L"Go to area in car";
		// ...
		case Game::PED_OBJECTIVE_GO_TO_CHAR_ON_FOOT: return L"Go to character on foot";
		// ...
		case Game::PED_OBJECTIVE_KILL_CHAR_ANY_MEANS: return L"Kill character any means";
		case Game::PED_OBJECTIVE_KILL_CHAR_ON_FOOT: return L"Kill character on foot";
		case Game::PED_OBJECTIVE_KILL_CHAR_IN_CAR: return L"Kill character in car";
		// ...
		case Game::PED_OBJECTIVE_BEAT_OR_ROB_CHAR: return L"Beat or rob character";
		case Game::PED_OBJECTIVE_GUARD_SPOT: return L"Guard spot";
		case Game::PED_OBJECTIVE_GUARD_AREA: return L"Guard area";
		case Game::PED_OBJECTIVE_WAIT_ON_FOOT: return L"Wait on foot";
		case Game::PED_OBJECTIVE_WAIT_IN_CAR: return L"Wait in car";
		// ...
		case Game::PED_OBJECTIVE_ENTER_CAR: return L"Enter car";
		case Game::PED_OBJECTIVE_LEAVE_CAR: return L"Leave car";
		case Game::PED_OBJECTIVE_ENTER_TRAIN: return L"Enter train";
		case Game::PED_OBJECTIVE_LEAVE_TRAIN: return L"Leave train";
		// ...
		case Game::PED_OBJECTIVE_PATROL_ON_FOOT: return L"Patrol on foot";
		// ...
		case Game::PED_OBJECTIVE_FOLLOW_CAR_IN_CAR: return L"Follow car in car";
		case Game::PED_OBJECTIVE_FOLLOW_CAR_ON_FOOT_WITH_OFFSET: return L"Follow car on foot with offset";
		case Game::PED_OBJECTIVE_FIRE_AT_OBJECT_FROM_VEHICLE: return L"Fire at object from vehicle";
		case Game::PED_OBJECTIVE_DESTROY_OBJECT: return L"Destroy object";
		case Game::PED_OBJECTIVE_DESTROY_CAR: return L"Destroy car";
		case Game::PED_OBJECTIVE_PUT_OUT_CAR_FIRE: return L"Put out car fire";
		// ...

		default:
			return L"Unknown " + std::to_wstring(static_cast<int32_t>(value));
		}
	}

	static bool AreEqual(Game::PED_OBJECTIVE a, Game::PED_OBJECTIVE b) {
		return a == b;
	}

protected:
	PedObjectiveConverter() = delete;
	~PedObjectiveConverter() = delete;
	PedObjectiveConverter(const PedObjectiveConverter&) = delete;
	PedObjectiveConverter& operator=(const PedObjectiveConverter&) = delete;
};

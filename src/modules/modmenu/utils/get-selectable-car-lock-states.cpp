#include "get-selectable-car-lock-states.h"

const std::vector<Game::CAR_LOCK_STATE>& ModMenuModule::Utils::GetSelectableCarLockStates()
{
	static const std::vector<Game::CAR_LOCK_STATE> carLockStates({
		Game::CAR_LOCK_STATE_UNLOCKED,
		Game::CAR_LOCK_STATE_LOCKOUT_THIEF,
		Game::CAR_LOCK_STATE_LOCKOUT_PLAYER,
		Game::CAR_LOCK_STATE_LOCKED
	});
	return carLockStates;
}

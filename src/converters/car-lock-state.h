#pragma once
#include <string>
#include "../game/game.h"

class CarLockStateConverter {
public:
	static std::wstring ConvertToString(Game::CAR_LOCK_STATE value) {

		switch (value) {
			case Game::CAR_LOCK_STATE_NO_LOCK: return L"No lock";
			case Game::CAR_LOCK_STATE_LOCKED: return L"Locked";
			case Game::CAR_LOCK_STATE_LOCKOUT_THIEF: return L"Lockout thief";
			case Game::CAR_LOCK_STATE_UNLOCKED: return L"Unlocked";
			case Game::CAR_LOCK_STATE_LOCKED_PERMANENTLY: return L"Locked permanently";
			case Game::CAR_LOCK_STATE_LOCKOUT_PLAYER: return L"Lockout player";
			default: return L"???";
		}
	}

	static bool AreEqual(Game::CAR_LOCK_STATE a, Game::CAR_LOCK_STATE b) {
		return a == b;
	}

protected:
	CarLockStateConverter() = delete;
	~CarLockStateConverter() = delete;
	CarLockStateConverter(const CarLockStateConverter&) = delete;
	CarLockStateConverter& operator=(const CarLockStateConverter&) = delete;
};

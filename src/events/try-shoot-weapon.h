#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched before any ped tries to shoot a weapon.
*/
class TryShootWeaponEvent : public Core::EventBase {
public:
	static bool Init();
	TryShootWeaponEvent(Game::WEAPON_STRUCT* weapon) : m_weapon(weapon) {}
	virtual ~TryShootWeaponEvent() override {};

	Game::WEAPON_STRUCT* GetWeapon() const { return m_weapon; }
private:
	Game::WEAPON_STRUCT* m_weapon;
};

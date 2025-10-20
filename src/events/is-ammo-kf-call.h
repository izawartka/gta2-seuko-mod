#pragma once
#include "../core/core.h"

/*
Dispatched when the game checks if ammo for a weapon is -1 (Kill Frenzy) before decreasing ammo.
*/
class IsAmmoKfCallEvent : public Core::EventBase {
public:
	static bool Init();
	IsAmmoKfCallEvent(bool value) : m_value(value), m_modifiedValue(value) {};
	virtual ~IsAmmoKfCallEvent() override {};

	bool GetValue() const { return m_value; }
	bool GetModifiedValue() const { return m_modifiedValue; }
	void SetModifiedValue(bool value) { m_modifiedValue = value; }
private:
	bool m_value;
	bool m_modifiedValue;
};

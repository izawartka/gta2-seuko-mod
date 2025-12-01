#pragma once
#include "../core/core.h"

/*
Dispatched when the game checks if a tile should be culled (not rendered because it would not be visible).
*/
class CullingCheckEvent : public Core::EventBase {
public:
	static bool Init();
	CullingCheckEvent(bool doCull) : m_doCull(doCull), m_modifiedDoCull(doCull) {};
	virtual ~CullingCheckEvent() override {};

	void SetModifiedDoCull(bool doCull) { m_modifiedDoCull = doCull; }
	bool GetDoCull() const { return m_doCull; }
	bool GetModifiedDoCull() const { return m_modifiedDoCull; }

private:
	bool m_doCull;
	bool m_modifiedDoCull;
};

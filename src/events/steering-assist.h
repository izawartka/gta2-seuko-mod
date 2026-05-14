#pragma once
#include "../core/core.h"

/*
Dispatched when the game is about to check a moving car and make small steering corrections to keep it in line with the road.
This behavior can be blocked by calling SetDoAssist(false)
*/
class SteeringAssistEvent : public Core::EventBase {
public:
	static bool Init();
	SteeringAssistEvent() {};
	virtual ~SteeringAssistEvent() override {};

	void SetDoAssist(bool doAssist) { m_doAssist = doAssist; }
	bool GetDoAssist() const { return m_doAssist; }

private:
	bool m_doAssist = true;
};

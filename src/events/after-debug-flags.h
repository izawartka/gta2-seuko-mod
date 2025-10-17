#pragma once
#include "../core/core.h"

/*
Dispatched after the game's debug flags have been loaded.
*/
class AfterDebugFlagsEvent : public Core::EventBase {
public:
	static bool Init();
	AfterDebugFlagsEvent() {};
	virtual ~AfterDebugFlagsEvent() override {};
};

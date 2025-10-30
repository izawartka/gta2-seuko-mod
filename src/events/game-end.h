#pragma once
#include "../core/core.h"

/*
Dispatched when the game is about to end. Some game structures may have already been destroyed, but Game object should still be valid.
*/
class PreGameEndEvent : public Core::EventBase {
public:
	static bool Init();
	PreGameEndEvent() {};
	virtual ~PreGameEndEvent() override {};
};

/*
Dispatched when the game has ended. Most of the game structures, including the Game object, have been destroyed.
*/
class GameEndEvent : public Core::EventBase {
public:
	static bool Init();
	GameEndEvent() {};
	virtual ~GameEndEvent() override {};
};

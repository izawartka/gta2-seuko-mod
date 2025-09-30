#pragma once
#include "../core/core.h"

/*
Dispatched every game tick, before any game logic is processed. Runs only when the game is unpaused.
*/
class PreGameTickEvent : public Core::EventBase {
public:
	static bool Init();
	PreGameTickEvent() {};
	virtual ~PreGameTickEvent() override {};
};

/*
Dispatched every game tick, after all game logic is processed. Runs only when the game is unpaused.
*/
class GameTickEvent : public Core::EventBase {
public:
	static bool Init();
	GameTickEvent() {};
	virtual ~GameTickEvent() override {};
};

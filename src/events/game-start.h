#pragma once
#include "../core/core.h"

/*
Dispatched when the game is about to start, before constructing basic game structures.
*/
class PreGameStartEvent : public Core::EventBase {
public:
	static bool Init();
	PreGameStartEvent() {};
	virtual ~PreGameStartEvent() override {};
};

/*
Dispatched when the game has just started, after all game structures have been constructed.
*/
class GameStartEvent : public Core::EventBase {
public:
	static bool Init();
	GameStartEvent() {};
	virtual ~GameStartEvent() override {};
};

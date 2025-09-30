#pragma once
#include "../core/core.h"

/*
Dispatched right before the UI is drawn. 
Things drawn here will appear on top of the game world and exploding scores but behind all other UI elements.
*/
class PreDrawUIEvent : public Core::EventBase {
public:
	static bool Init();
	PreDrawUIEvent() {};
	virtual ~PreDrawUIEvent() override {};
};

/*
Dispatched right after the UI is drawn. 
Things drawn here will appear on top of all other UI elements (excluding mike's test).
*/
class DrawUIEvent : public Core::EventBase {
public:
	static bool Init();
	DrawUIEvent() {};
	virtual ~DrawUIEvent() override {};
};

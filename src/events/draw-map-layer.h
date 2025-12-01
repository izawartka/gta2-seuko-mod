#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched right before tiles for each layer of the map are drawn.
Layer index is accessible via Game::Memory::GetCurrentlyDrawnMapLayerIndex()
Things drawn here will appear below that layer.
Drawing can be skipped by setting skipDraw to true.
*/
class PreDrawMapLayerEvent : public Core::EventBase {
public:
	static bool Init();
	PreDrawMapLayerEvent(Game::S12* s12) : m_s12(s12) {};
	virtual ~PreDrawMapLayerEvent() override {};

	Game::S12* GetS12() const { return m_s12; }
	void SetSkipDraw(bool doSkip) { m_doSkip = doSkip; }
	bool GetSkipDraw() const { return m_doSkip; }

private:
	Game::S12* m_s12;
	bool m_doSkip = false;
};

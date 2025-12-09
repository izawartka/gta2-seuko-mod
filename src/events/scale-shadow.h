#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched when the game is about to calculate shadow distance for a sprite.
*/
class ScaleShadowEvent : public Core::EventBase {
public:
	static bool Init();
	ScaleShadowEvent(Game::SCR_f scale) : m_scale(scale), m_modifiedScale(scale) {}
	virtual ~ScaleShadowEvent() override {}

	Game::SCR_f GetScale() const { return m_scale; }
	Game::SCR_f GetModifiedScale() const { return m_modifiedScale; }
	void SetModifiedScale(Game::SCR_f scale) { m_modifiedScale = scale; }

private:
	Game::SCR_f m_scale;
	Game::SCR_f m_modifiedScale;
};

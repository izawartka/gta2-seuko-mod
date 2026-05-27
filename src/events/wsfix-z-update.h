#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched when the game is unpaused and just after main (ph1) or aux (ph3) camera gets copied to view (ph2) camera.
At this point, ThirteenAG's WidescreenFix, if running, may have modified the camera height (z position).
HasWidescreenFix() method can be used to determine if WidescreenFix is installed and enabled.
GetZOffset() method can be used to read the camera height increase caused by WidescreenFix.
*/
class WSFixZUpdateEvent : public Core::EventBase {
public:
	static bool Init();
	WSFixZUpdateEvent(bool hasWidescreenFix, Game::SCR_f zOffset)
		: m_hasWidescreenFix(hasWidescreenFix), m_zOffset(zOffset) {}
	virtual ~WSFixZUpdateEvent() override {};

	bool HasWidescreenFix() const { return m_hasWidescreenFix; }
	Game::SCR_f GetZOffset() const { return m_zOffset; }

private:
	bool m_hasWidescreenFix = false;
	Game::SCR_f m_zOffset = 0;
};

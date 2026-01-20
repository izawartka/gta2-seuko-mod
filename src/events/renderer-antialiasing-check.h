#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched when the renderer checks if a given quad or tile should be drawn antialiased.
Event will fail to initialize if the renderer has not been loaded yet.
*/
class RendererAntialiasingCheckEvent : public Core::EventBase {
public:
	static bool Init();
	RendererAntialiasingCheckEvent(uint32_t flags, bool doAntialiasing)
		: m_flags(flags), m_doAntialiasing(doAntialiasing), m_modifiedDoAntialiasing(doAntialiasing) { };
	virtual ~RendererAntialiasingCheckEvent() override {};

	uint32_t GetFlags() const { return m_flags; }

	bool GetDoAntialiasing() const { return m_doAntialiasing; }
	bool GetModifiedDoAntialiasing() const { return m_modifiedDoAntialiasing; }
	void SetModifiedDoAntialiasing(bool doAntialiasing) { m_modifiedDoAntialiasing = doAntialiasing; }

private:
	uint32_t m_flags;
	bool m_doAntialiasing;
	bool m_modifiedDoAntialiasing;
};

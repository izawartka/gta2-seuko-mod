#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched after the renderer is loaded.
*/
class RendererLoadEvent : public Core::EventBase {
public:
	static bool Init();
	RendererLoadEvent() {};
	virtual ~RendererLoadEvent() override {};
};

/*
Dispatched when the game calls renderer gbh_DrawTile function.
Event will fail to initialize if the renderer has not been loaded yet.
*/
class RendererDrawTileEvent : public Core::EventBase {
public:
	static bool Init();
	RendererDrawTileEvent(uint32_t flags, Game::GTAVertex* vertices) 
		: m_flags(flags), m_vertices(vertices) {};
	virtual ~RendererDrawTileEvent() override {};

	uint32_t GetFlags() const { return m_flags; }
	Game::GTAVertex* GetVertices() const { return m_vertices; }
	void SetVertices(Game::GTAVertex* vertices) { m_vertices = vertices; }

private:
	uint32_t m_flags;
	Game::GTAVertex* m_vertices;
};

/*
Dispatched when the game calls renderer gbh_DrawQuad function.
Event will fail to initialize if the renderer has not been loaded yet.
*/
class RendererDrawQuadEvent : public Core::EventBase {
public:
	static bool Init();
	RendererDrawQuadEvent(uint32_t flags, Game::GTAVertex* vertices)
		: m_flags(flags), m_vertices(vertices) {};
	virtual ~RendererDrawQuadEvent() override {};

	uint32_t GetFlags() const { return m_flags; }
	Game::GTAVertex* GetVertices() const { return m_vertices; }
	void SetVertices(Game::GTAVertex* vertices) { m_vertices = vertices; }

private:
	uint32_t m_flags;
	Game::GTAVertex* m_vertices;
};

/*
Dispatched when the game calls renderer gbh_DrawTriangle function.
Event will fail to initialize if the renderer has not been loaded yet.
*/

class RendererDrawTriangleEvent : public Core::EventBase {
public:
	static bool Init();
	RendererDrawTriangleEvent(uint32_t flags, Game::GTAVertex* vertices)
		: m_flags(flags), m_vertices(vertices) {};
	virtual ~RendererDrawTriangleEvent() override {};

	uint32_t GetFlags() const { return m_flags; }
	Game::GTAVertex* GetVertices() const { return m_vertices; }
	void SetVertices(Game::GTAVertex* vertices) { m_vertices = vertices; }

private:
	uint32_t m_flags;
	Game::GTAVertex* m_vertices;
};

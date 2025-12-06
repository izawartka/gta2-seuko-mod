#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched when the game is about to copy camera targetPos2 to targetPos.
Setting DoApply to false will *probably safely* freeze the camera target position.
*/
class CameraPosApplyEvent : public Core::EventBase {
public:
	static bool Init();
	CameraPosApplyEvent(Game::Camera* camera) : m_camera(camera) {};
	virtual ~CameraPosApplyEvent() override {};

	Game::Camera* GetCamera() const { return m_camera; }
	bool GetDoApply() const { return m_doApply; }
	void SetDoApply(bool doApply) { m_doApply = doApply; }

private:
	Game::Camera* m_camera;
	bool m_doApply = true;
};

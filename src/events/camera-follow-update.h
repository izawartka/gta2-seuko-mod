#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched when the camera is performing a follow update. Follow state can be modified.
*/
class CameraFollowUpdateEvent : public Core::EventBase {
public:
	static bool Init();
	CameraFollowUpdateEvent(Game::Camera* camera, Game::CAMERA_FOLLOW_STATE followState) 
		: m_camera(camera), m_followState(followState), m_modifiedFollowState(followState) { }
	
	virtual ~CameraFollowUpdateEvent() override {};

	Game::Camera* GetCamera() const { return m_camera; }

	Game::CAMERA_FOLLOW_STATE GetFollowState() const { return m_followState; }
	Game::CAMERA_FOLLOW_STATE GetModifiedFollowState() const { return m_modifiedFollowState; }
	void SetModifiedFollowState(Game::CAMERA_FOLLOW_STATE state) { m_modifiedFollowState = state; }

private:
	Game::Camera* m_camera;
	Game::CAMERA_FOLLOW_STATE m_followState;
	Game::CAMERA_FOLLOW_STATE m_modifiedFollowState;
};

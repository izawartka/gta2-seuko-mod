#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Base class for PreUpdateCarCameraEvent and PostUpdateCarCameraEvent. Not to be used directly.
*/
class UpdateCarCameraEventBase : public Core::EventBase {
protected:
	UpdateCarCameraEventBase(
		Game::Camera* camera,
		Game::Car* car,
		Game::SCR_f* cameraX,
		Game::SCR_f* cameraY,
		Game::SCR_f* cameraZ
	) : m_camera(camera), m_car(car), m_cameraX(cameraX), 
		m_cameraY(cameraY), m_cameraZ(cameraZ) {}
	virtual ~UpdateCarCameraEventBase() override {};

public:
	Game::Camera* GetCamera() const { return m_camera; }
	Game::Car* GetCar() const { return m_car; }
	Game::SCR_f GetCameraX() const { return *m_cameraX; }
	void SetCameraX(Game::SCR_f x) { *m_cameraX = x; }
	Game::SCR_f* GetCameraXPtr() { return m_cameraX; }
	Game::SCR_f GetCameraY() const { return *m_cameraY; }
	void SetCameraY(Game::SCR_f y) { *m_cameraY = y; }
	Game::SCR_f* GetCameraYPtr() { return m_cameraY; }
	Game::SCR_f GetCameraZ() const { return *m_cameraZ; }
	void SetCameraZ(Game::SCR_f z) { *m_cameraZ = z; }
	Game::SCR_f* GetCameraZPtr() { return m_cameraZ; }

	Game::SCR_Vector3 GetCameraPos() const {
		return Game::SCR_Vector3{ *m_cameraX, *m_cameraY, *m_cameraZ };
	}

	void SetCameraPos(const Game::SCR_Vector3& position) {
		*m_cameraX = position.x;
		*m_cameraY = position.y;
		*m_cameraZ = position.z;
	}

private:
	Game::Camera* m_camera;
	Game::Car* m_car;
	Game::SCR_f* m_cameraX;
	Game::SCR_f* m_cameraY;
	Game::SCR_f* m_cameraZ;
};

/*
Dispatched before the game applies car (including train) camera effects.
*/
class PreUpdateCarCameraEvent : public UpdateCarCameraEventBase {
public:
	static bool Init();
	PreUpdateCarCameraEvent(
		Game::Camera* camera,
		Game::Car* car,
		Game::SCR_f* cameraX,
		Game::SCR_f* cameraY,
		Game::SCR_f* cameraZ
	) : UpdateCarCameraEventBase(camera, car, cameraX, cameraY, cameraZ) {}
	virtual ~PreUpdateCarCameraEvent() override {};

	void SetDoUpdate(bool doUpdate) { m_doUpdate = doUpdate; }
	bool GetDoUpdate() const { return m_doUpdate; }

private:
	bool m_doUpdate = true;
};

/*
Dispatched right after the game applies car (including train) camera effects.
*/
class PostUpdateCarCameraEvent : public UpdateCarCameraEventBase {
public:
	static bool Init();
	PostUpdateCarCameraEvent(
		Game::Camera* camera,
		Game::Car* car,
		Game::SCR_f* cameraX,
		Game::SCR_f* cameraY,
		Game::SCR_f* cameraZ
	) : UpdateCarCameraEventBase(camera, car, cameraX, cameraY, cameraZ) {}
	virtual ~PostUpdateCarCameraEvent() override {};
};

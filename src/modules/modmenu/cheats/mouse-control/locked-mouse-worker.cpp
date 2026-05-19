#include "locked-mouse-worker.h"
#include "mouse-control.h"
#include "../../common.h"

static constexpr float LOCKED_MOUSE_SMOOTING = 0.2f;

void ModMenuModule::LockedMouseWorker::Start()
{
	spdlog::debug("LockedMouseWorker: Starting");

	AddEventListener<MouseModule::MouseLockedMoveEvent>(&LockedMouseWorker::OnMouseLockedMove);

	MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
	mouseManager->SetLocked(true);
	mouseManager->SetCursorVisibility(MouseModule::CursorVisibility::ForceInvisible);
	m_running = true;
}

void ModMenuModule::LockedMouseWorker::Stop()
{
	spdlog::debug("LockedMouseWorker: Stopping");

	RemoveEventListener<MouseModule::MouseLockedMoveEvent>();

	MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
	mouseManager->SetLocked(false);
	mouseManager->SetCursorVisibility(MouseModule::CursorVisibility::Unmodified);
	m_running = false;
}

void ModMenuModule::LockedMouseWorker::Update()
{
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	float deltaX = mouseControlCheat->GetDeltaX();
	float deltaY = mouseControlCheat->GetDeltaY();

	deltaX *= (1.0f - LOCKED_MOUSE_SMOOTING);
	deltaY *= (1.0f - LOCKED_MOUSE_SMOOTING);

	mouseControlCheat->SetDeltaX(deltaX);
	mouseControlCheat->SetDeltaY(deltaY);
}

void ModMenuModule::LockedMouseWorker::OnMouseLockedMove(MouseModule::MouseLockedMoveEvent& event)
{
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	float sensitivity = mouseControlCheat->GetOptions().rotateModeSensitivity;

	float deltaX = mouseControlCheat->GetDeltaX();
	float deltaY = mouseControlCheat->GetDeltaY();

	deltaX += (event.GetDelta().x * sensitivity);
	deltaY += (event.GetDelta().y * sensitivity);

	mouseControlCheat->SetDeltaX(deltaX);
	mouseControlCheat->SetDeltaY(deltaY);
}

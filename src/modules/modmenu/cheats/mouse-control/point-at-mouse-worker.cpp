#include "point-at-mouse-worker.h"
#include "mouse-control.h"
#include "../camera/camera.h"
#include "../../common.h"
#include "../../utils/angle-utils.h"

void ModMenuModule::PointAtMouseWorker::Start()
{
	spdlog::debug("PointAtMouseWorker: Starting");

	AddEventListener<MouseModule::MouseMoveEvent>(&PointAtMouseWorker::OnMouseMove);

	MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
	mouseManager->SetLocked(false); // ensure
	mouseManager->SetCursorVisibility(MouseModule::CursorVisibility::ForceVisible);
	m_running = true;

	UpdateLastMousePosition();
}

void ModMenuModule::PointAtMouseWorker::Stop()
{
	spdlog::debug("PointAtMouseWorker: Stopping");

	RemoveEventListener<MouseModule::MouseMoveEvent>();

	MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
	mouseManager->SetCursorVisibility(MouseModule::CursorVisibility::Unmodified);
	m_running = false;
}

void ModMenuModule::PointAtMouseWorker::Update()
{
	float deltaRotation = 0.0f;
	float currentRotation = GetPlayerPedRotation();

	std::optional<float> targetRotationOpt = GetTargetRotation(m_lastMousePosition);
	if (targetRotationOpt.has_value()) {
		deltaRotation = Utils::Angle::GetShortestAngleDifference(currentRotation, targetRotationOpt.value());
	}

	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	mouseControlCheat->SetDeltaX(deltaRotation);
}

void ModMenuModule::PointAtMouseWorker::OnMouseMove(MouseModule::MouseMoveEvent& event)
{
	MouseModule::MousePosition pos = event.GetPosition();
	m_lastMousePosition = MouseModule::MouseManager::ToNormalizedPosition(pos);
}

void ModMenuModule::PointAtMouseWorker::UpdateLastMousePosition()
{
	MouseModule::MousePosition currentPos = MouseModule::MouseManager::FetchMouseState().position;
	m_lastMousePosition = MouseModule::MouseManager::ToNormalizedPosition(currentPos);
}

std::optional<float> ModMenuModule::PointAtMouseWorker::GetTargetRotation(MouseModule::NormalizedMousePosition normalizedPos)
{
	normalizedPos.x -= 0.5f;
	normalizedPos.y -= 0.5f;
	normalizedPos.x *= -1.0f;

	normalizedPos.x *= MouseModule::MouseManager::GetClientAreaAspectRatio();
	float targetAngle;

	CameraCheat* cameraCheat = CameraCheat::GetInstance();
	if (cameraCheat->IsEnabled()) {
		CameraCheatOptions cameraOptions = cameraCheat->GetOptions();
		normalizedPos.x *= std::cos(cameraOptions.cameraTransform.horizontalAngleRad);
		if (normalizedPos.x == 0.0f && normalizedPos.y == 0.0f) return std::nullopt;
		targetAngle = std::atan2(normalizedPos.x, normalizedPos.y);

		float cameraRotation = cameraCheat->GetOptions().cameraTransform.verticalAngleRad;
		targetAngle -= cameraRotation;
	}
	else {
		targetAngle = std::atan2(normalizedPos.x, normalizedPos.y);
	}

	return Utils::Angle::NormalizeAngle(targetAngle);
}

float ModMenuModule::PointAtMouseWorker::GetPlayerPedRotation()
{
	Game::Ped* playerPed = Game::Utils::GetPlayerCurrentPed();
	if (!playerPed || !playerPed->gameObject) return 0;

	return -Game::Utils::FromGTAAngleToRad(playerPed->gameObject->spriteRotation);
}

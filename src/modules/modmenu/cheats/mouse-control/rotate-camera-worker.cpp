#include "rotate-camera-worker.h"
#include "mouse-control.h"
#include "../camera/camera.h"
#include "../../common.h"
#include "../../utils/angle-utils.h"

static constexpr float ROTATE_CAMERA_SENSITIVITY = 0.75f;

void ModMenuModule::RotateCameraWorker::Start()
{
	spdlog::debug("RotateCameraWorker: Starting");
	EnsureFPROHandle();

	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	mouseControlCheat->SetDeltaX(0.0f);
	m_running = true;
}

void ModMenuModule::RotateCameraWorker::Stop()
{
	spdlog::debug("RotateCameraWorker: Stopping");

	FreeFPROHandle();
	m_running = false;
}

void ModMenuModule::RotateCameraWorker::Update()
{
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	CameraCheat* cameraCheat = CameraCheat::GetInstance();

	float deltaX = mouseControlCheat->GetDeltaX();
	if (deltaX == 0.0f) return;
	mouseControlCheat->SetDeltaX(0.0f);

	if (!EnsureFPROHandle()) return;

	float newOffset = Utils::Angle::NormalizeAngle(m_currentOffset - deltaX * ROTATE_CAMERA_SENSITIVITY);

	if (cameraCheat->SetFPRO(m_FPROHandleId, newOffset)) {
		m_currentOffset = newOffset;
	}
}

bool ModMenuModule::RotateCameraWorker::EnsureFPROHandle()
{
	CameraCheat* cameraCheat = CameraCheat::GetInstance();
	if (!cameraCheat->IsEnabled()) return false;

	if (m_FPROHandleId != -1 && cameraCheat->IsFPROHandleValid(m_FPROHandleId)) {
		return true;
	}

	m_FPROHandleId = cameraCheat->CreateFPROHandle();
	if (m_FPROHandleId == -1) {
		return false;
	}

	CameraCheatOptions cameraOptions = cameraCheat->GetOptions();
	m_currentOffset = cameraOptions.followPedRotationOffset;

	return true;
}

void ModMenuModule::RotateCameraWorker::FreeFPROHandle()
{
	m_currentOffset = 0.0f;

	CameraCheatFPROHandleId handleId = m_FPROHandleId;
	m_FPROHandleId = -1;

	CameraCheat* cameraCheat = CameraCheat::GetInstance();
	if (!cameraCheat->IsEnabled()) {
		return;
	}

	cameraCheat->FreeFPROHandle(handleId);
}

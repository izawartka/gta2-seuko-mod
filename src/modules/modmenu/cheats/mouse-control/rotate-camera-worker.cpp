#include "rotate-camera-worker.h"
#include "mouse-control.h"
#include "../camera/camera.h"
#include "../../common.h"
#include "../../utils/angle-utils.h"

static constexpr float ROTATE_CAMERA_SENSITIVITY = 0.75f;

void ModMenuModule::RotateCameraWorker::Start()
{
	spdlog::debug("RotateCameraWorker: Starting");

	SaveInitialCameraOffset();

	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	mouseControlCheat->SetDeltaX(0.0f);
	m_running = true;
}

void ModMenuModule::RotateCameraWorker::Stop()
{
	spdlog::debug("RotateCameraWorker: Stopping");

	RestoreInitialCameraOffset();

	m_running = false;
}

void ModMenuModule::RotateCameraWorker::Update()
{
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	CameraCheat* cameraCheat = CameraCheat::GetInstance();

	float deltaX = mouseControlCheat->GetDeltaX();
	if (deltaX == 0.0f) return;
	mouseControlCheat->SetDeltaX(0.0f);

	if (!cameraCheat->IsEnabled()) return;

	CameraCheatOptions cameraOptions = cameraCheat->GetOptions();
	if (!cameraOptions.followPedRotation) return;

	float newOffset = cameraOptions.followPedRotationOffset - deltaX * ROTATE_CAMERA_SENSITIVITY;
	cameraOptions.followPedRotationOffset = Utils::Angle::NormalizeAngle(newOffset);

	cameraCheat->SetOptions(cameraOptions);
}

void ModMenuModule::RotateCameraWorker::SaveInitialCameraOffset()
{
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	CameraCheat* cameraCheat = CameraCheat::GetInstance();

	if (!cameraCheat->IsEnabled()) return;

	CameraCheatOptions cameraOptions = cameraCheat->GetOptions();
	if (!cameraOptions.followPedRotation) return;

	m_initialCameraOffset = cameraOptions.followPedRotationOffset;
}

void ModMenuModule::RotateCameraWorker::RestoreInitialCameraOffset() const
{
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	CameraCheat* cameraCheat = CameraCheat::GetInstance();

	if (!cameraCheat->IsEnabled()) return;

	CameraCheatOptions cameraOptions = cameraCheat->GetOptions();
	if (!cameraOptions.followPedRotation) return;

	cameraOptions.followPedRotationOffset = m_initialCameraOffset;
	cameraCheat->SetOptions(cameraOptions);
}

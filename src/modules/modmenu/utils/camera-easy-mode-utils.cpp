#include "camera-easy-mode-utils.h"
#include "../cheats/camera/camera.h"
#include "../cheats/camera/camera-pos.h"
#include "../cheats/camera/clear-screen.h"
#include "../cheats/camera/disable-culling.h"
#include "../cheats/camera/shadows-fix.h"

static const ModMenuModule::CameraCheatOptions rotateCameraOptions = {
	{ 0.0f, 0.0f, 5.0f }, // cameraTransform
	false, // customCulling
	false, // customRenderQueue
	true, // followPedRotation
	0.5f, // followPedRotationLerpFactor
	20 // renderDistance
};

static const ModMenuModule::CameraPosCheatOptions rotateCameraPosOptions = {
	{ ModMenuModule::CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false }, // x
	{ ModMenuModule::CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false }, // y
	{ ModMenuModule::CameraPosCheatMode::IncrementTargetBy, Game::Utils::FromFloat(5.0f), false }, // z
	{ ModMenuModule::CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false }, // zoom
	true
};

static const ModMenuModule::CameraCheatOptions threeDimensionalCameraOptions = {
	{ (static_cast<float>(M_PI) / 4.0f), 0.0f, 10.0f }, // cameraTransform
	true, // customCulling
	true, // customRenderQueue
	true, // followPedRotation
	0.5f, // followPedRotationLerpFactor
	20 // renderDistance
};

static const ModMenuModule::CameraPosCheatOptions threeDimensionalCameraPosOptions = {
	{ ModMenuModule::CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false }, // x
	{ ModMenuModule::CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false }, // y
	{ ModMenuModule::CameraPosCheatMode::IncrementTargetBy, Game::Utils::FromFloat(10.0f), false }, // z
	{ ModMenuModule::CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false }, // zoom
	true
};

static void EnableCameraCheatKeepVerticalAngle(ModMenuModule::CameraCheat& cheat, const ModMenuModule::CameraCheatOptions& options)
{
	if (cheat.IsEnabled()) {
		float prevAngle = cheat.GetOptions().cameraTransform.verticalAngleRad;
		ModMenuModule::CameraCheatOptions updatedOptions = options;
		updatedOptions.cameraTransform.verticalAngleRad = prevAngle;
		cheat.SetOptions(updatedOptions);
	}
	else {
		cheat.SetEnabled(true);
		cheat.SetOptions(options);
	}
}

ModMenuModule::Utils::CameraEasyMode::CameraEasyMode ModMenuModule::Utils::CameraEasyMode::GetCurrentMode()
{
	CameraCheat* cameraCheat = CameraCheat::GetInstance();
	CameraPosCheat* cameraPosCheat = CameraPosCheat::GetInstance();
	ClearScreenCheat* clearScreenCheat = ClearScreenCheat::GetInstance();
	DisableCullingCheat* disableCullingCheat = DisableCullingCheat::GetInstance();
	ShadowsFixCheat* shadowsFixCheat = ShadowsFixCheat::GetInstance();

	bool cameraCheatEnabled = cameraCheat->IsEnabled();
	bool cameraPosCheatEnabled = cameraPosCheat->IsEnabled();
	bool clearScreenCheatEnabled = clearScreenCheat->IsEnabled();
	bool disableCullingCheatEnabled = disableCullingCheat->IsEnabled();
	bool shadowsFixCheatEnabled = shadowsFixCheat->IsEnabled();

	bool allDisabled = 
		!cameraCheatEnabled &&
		!cameraPosCheatEnabled &&
		!clearScreenCheatEnabled &&
		!disableCullingCheatEnabled &&
		!shadowsFixCheatEnabled;

	if (allDisabled) {
		return CameraEasyMode::Unmodified;
	}

	bool fixShadowsOnlyEnabled =
		!cameraCheatEnabled &&
		!cameraPosCheatEnabled &&
		!clearScreenCheatEnabled &&
		!disableCullingCheatEnabled &&
		shadowsFixCheatEnabled;

	if (fixShadowsOnlyEnabled) {
		return CameraEasyMode::FixShadows;
	}

	bool camDisabled = (!cameraCheat || !cameraCheat->IsEnabled());
	bool posDisabled = (!cameraPosCheat || !cameraPosCheat->IsEnabled());

	if (camDisabled || posDisabled) {
		return CameraEasyMode::Custom;
	}

	bool rotateOnlyEnabled =
		clearScreenCheatEnabled &&
		!disableCullingCheatEnabled &&
		shadowsFixCheatEnabled;

	const CameraCheatOptions& camOptions = cameraCheat->GetOptions();
	const CameraPosCheatOptions& posOptions = cameraPosCheat->GetOptions();

	bool isRotate =
		rotateOnlyEnabled &&
		camOptions.SmartEquals(rotateCameraOptions) &&
		posOptions.SmartEquals(rotateCameraPosOptions);

	if (isRotate) {
		return CameraEasyMode::Rotate;
	}

	bool allEnabled =
		clearScreenCheatEnabled &&
		disableCullingCheatEnabled &&
		shadowsFixCheatEnabled;

	if (!allEnabled) {
		return CameraEasyMode::Custom;
	}

	bool isThreeDimensional =
		camOptions.SmartEquals(threeDimensionalCameraOptions) &&
		posOptions.SmartEquals(threeDimensionalCameraPosOptions);

	if (isThreeDimensional) {
		return CameraEasyMode::ThreeDimensional;
	}

	return CameraEasyMode::Custom;
}

bool ModMenuModule::Utils::CameraEasyMode::SetCurrentMode(CameraEasyMode newMode)
{
	CameraCheat* cameraCheat = CameraCheat::GetInstance();
	CameraPosCheat* cameraPosCheat = CameraPosCheat::GetInstance();
	ClearScreenCheat* clearScreenCheat = ClearScreenCheat::GetInstance();
	DisableCullingCheat* disableCullingCheat = DisableCullingCheat::GetInstance();
	ShadowsFixCheat* shadowsFixCheat = ShadowsFixCheat::GetInstance();

	switch (newMode) {
	case CameraEasyMode::Unmodified:
		if(cameraCheat->IsEnabled()) cameraCheat->SetOptions({});
		cameraCheat->SetEnabled(false);
		if (cameraPosCheat->IsEnabled()) {
			cameraPosCheat->SetOptions({});
			cameraPosCheat->SnapAndDisable();
		}
		clearScreenCheat->SetEnabled(false);
		disableCullingCheat->SetEnabled(false);
		shadowsFixCheat->SetEnabled(false);
		return true;

	case CameraEasyMode::FixShadows:
		if (cameraCheat->IsEnabled()) cameraCheat->SetOptions({});
		cameraCheat->SetEnabled(false);
		if (cameraPosCheat->IsEnabled()) {
			cameraPosCheat->SetOptions({});
			cameraPosCheat->SnapAndDisable();
		}
		clearScreenCheat->SetEnabled(false);
		disableCullingCheat->SetEnabled(false);
		shadowsFixCheat->SetEnabled(true);
		return true;

	case CameraEasyMode::Rotate:
		EnableCameraCheatKeepVerticalAngle(*cameraCheat, rotateCameraOptions);
		cameraPosCheat->SetEnabled(true);
		cameraPosCheat->SetOptions(rotateCameraPosOptions);
		cameraPosCheat->SnapToTargetPos();
		clearScreenCheat->SetEnabled(true);
		disableCullingCheat->SetEnabled(false);
		shadowsFixCheat->SetEnabled(true);
		return true;

	case CameraEasyMode::ThreeDimensional:
		EnableCameraCheatKeepVerticalAngle(*cameraCheat, threeDimensionalCameraOptions);
		cameraPosCheat->SetEnabled(true);
		cameraPosCheat->SetOptions(threeDimensionalCameraPosOptions);
		cameraPosCheat->SnapToTargetPos();
		clearScreenCheat->SetEnabled(true);
		disableCullingCheat->SetEnabled(true);
		shadowsFixCheat->SetEnabled(true);
		return true;

	case CameraEasyMode::Custom:
		spdlog::warn("CameraEasyMode::SetCurrentMode: Cannot set to Custom mode directly");
		return false;

	default:
		spdlog::warn("CameraEasyMode::SetCurrentMode: Unknown CameraEasyMode value {}", static_cast<int>(newMode));
		return false;
	}

	return true;
}

const std::vector<ModMenuModule::Utils::CameraEasyMode::CameraEasyMode>& ModMenuModule::Utils::CameraEasyMode::GetSelectableModes()
{
	static const std::vector<CameraEasyMode> modes = {
		CameraEasyMode::Unmodified,
		CameraEasyMode::FixShadows,
		CameraEasyMode::Rotate,
		CameraEasyMode::ThreeDimensional,
	};
	return modes;
}

#include "left-right-worker.h"
#include "mouse-control.h"
#include "../../common.h"

static constexpr float LEFT_RIGHT_INPUT_THRESHOLD = 0.15f;
static constexpr float LEFT_RIGHT_DELTA_DECLINE = 0.20943928f;

static constexpr size_t LEFT_CONTROL_INDEX = Game::Utils::GetControlIndex(Game::KEYBOARD_STATE_LEFT);
static constexpr size_t RIGHT_CONTROL_INDEX = Game::Utils::GetControlIndex(Game::KEYBOARD_STATE_RIGHT);

void ModMenuModule::LeftRightWorker::Start()
{
	spdlog::debug("LeftRightWorker: Starting");

	if (!CreateControlHandles()) return;

	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	mouseControlCheat->SetDeltaX(0.0f);
	m_running = true;
}

void ModMenuModule::LeftRightWorker::Stop()
{
	spdlog::debug("LeftRightWorker: Stopping");

	FreeControlHandles();
	m_running = false;
}

void ModMenuModule::LeftRightWorker::Update()
{
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();

	float deltaX = mouseControlCheat->GetDeltaX();
	char direction = GetDirection(deltaX);
	bool shouldPressLeft = direction < 0;
	bool shouldPressRight = direction > 0;

	forceControlsCheat->SetControlState(m_leftControlHandle, shouldPressLeft ? ForceControlState::ForceDown : ForceControlState::ForceUp);
	forceControlsCheat->SetControlState(m_rightControlHandle, shouldPressRight ? ForceControlState::ForceDown : ForceControlState::ForceUp);

	deltaX -= (direction * LEFT_RIGHT_DELTA_DECLINE);
	mouseControlCheat->SetDeltaX(deltaX);
}

bool ModMenuModule::LeftRightWorker::CreateControlHandles()
{
	bool anyUsesGamepad = ForceControlsCheat::CheckUsesGamepad(LEFT_CONTROL_INDEX) 
		|| ForceControlsCheat::CheckUsesGamepad(RIGHT_CONTROL_INDEX);

	if (anyUsesGamepad) {
		spdlog::error("LeftRightWorker: cannot create control handle for left/right controls because they are used by gamepad controls");
		MouseControlCheat::GetInstance()->ShowGamepadControlsToast();
		return false;
	}

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();

	m_leftControlHandle = forceControlsCheat->CreateControlHandle(LEFT_CONTROL_INDEX);
	if (m_leftControlHandle == -1) {
		spdlog::error("LeftRightWorker: failed to create control handle for the control: left");
		return false;
	}

	m_rightControlHandle = forceControlsCheat->CreateControlHandle(RIGHT_CONTROL_INDEX);
	if (m_rightControlHandle == -1) {
		spdlog::error("LeftRightWorker: failed to create control handle for the control: right");
		forceControlsCheat->FreeControlHandle(m_leftControlHandle);
		m_leftControlHandle = -1;
		return false;
	}

	return true;
}

void ModMenuModule::LeftRightWorker::FreeControlHandles()
{
	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();

	if (m_leftControlHandle != -1) {
		forceControlsCheat->SetControlState(m_leftControlHandle, ForceControlState::Unmodified);
		forceControlsCheat->FreeControlHandle(m_leftControlHandle);
		m_leftControlHandle = -1;
	}

	if (m_rightControlHandle != -1) {
		forceControlsCheat->SetControlState(m_rightControlHandle, ForceControlState::Unmodified);
		forceControlsCheat->FreeControlHandle(m_rightControlHandle);
		m_rightControlHandle = -1;
	}
}

char ModMenuModule::LeftRightWorker::GetDirection(float delta)
{
	if (std::abs(delta) < LEFT_RIGHT_INPUT_THRESHOLD) {
		return 0;
	}
	return delta > 0 ? 1 : -1;
}

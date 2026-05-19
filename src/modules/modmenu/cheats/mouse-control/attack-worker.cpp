#include "attack-worker.h"
#include "mouse-control.h"
#include "../../common.h"

static constexpr size_t ATTACK_CONTROL_INDEX = Game::Utils::GetControlIndex(Game::KEYBOARD_STATE_ATTACK);

void ModMenuModule::AttackWorker::Start()
{
	spdlog::debug("AttackWorker: Starting");

	if (!CreateControlHandle()) return;
	AddEventListener<MouseModule::MouseButtonDownEvent>(&AttackWorker::OnMouseButtonDown);
	AddEventListener<MouseModule::MouseButtonUpEvent>(&AttackWorker::OnMouseButtonUp);
	m_running = true;

	ForceUpdate();
}

void ModMenuModule::AttackWorker::Stop()
{
	spdlog::debug("AttackWorker: Stopping");

	RemoveEventListener<MouseModule::MouseButtonDownEvent>();
	RemoveEventListener<MouseModule::MouseButtonUpEvent>();
	FreeControlHandle();
	m_running = false;
}

void ModMenuModule::AttackWorker::OnMouseButtonDown(MouseModule::MouseButtonDownEvent& event)
{
	if (event.GetButton() != MouseModule::MouseButton::Left) return;

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_attackControlHandle, ForceControlState::ForceDown);
}

void ModMenuModule::AttackWorker::OnMouseButtonUp(MouseModule::MouseButtonUpEvent & event)
{
	if (event.GetButton() != MouseModule::MouseButton::Left) return;

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_attackControlHandle, ForceControlState::Unmodified);
}

bool ModMenuModule::AttackWorker::CreateControlHandle()
{
	if (ForceControlsCheat::CheckUsesGamepad(ATTACK_CONTROL_INDEX)) {
		spdlog::error("AttackWorker: cannot create control handle for attack control because it is used by gamepad controls");
		MouseControlCheat::GetInstance()->ShowGamepadControlsToast();
		return false;
	}

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();

	m_attackControlHandle = forceControlsCheat->CreateControlHandle(ATTACK_CONTROL_INDEX);
	if (m_attackControlHandle == -1) {
		spdlog::error("AttackWorker: failed to create control handle for the control: attack");
		return false;
	}

	return true;
}

void ModMenuModule::AttackWorker::FreeControlHandle()
{
	if (m_attackControlHandle == -1) return;

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_attackControlHandle, ForceControlState::Unmodified);
	forceControlsCheat->FreeControlHandle(m_attackControlHandle);
	m_attackControlHandle = -1;
}

void ModMenuModule::AttackWorker::ForceUpdate() const
{
	MouseModule::MouseState mouseState = MouseModule::MouseManager::FetchMouseState();
	bool shouldAttack = mouseState.leftButtonDown;

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_attackControlHandle, shouldAttack ? ForceControlState::ForceDown : ForceControlState::Unmodified);
}

#include "force-controls.h"
#include "../cheat-registry.h"

ModMenuModule::ForceControlsCheat* ModMenuModule::ForceControlsCheat::m_instance = nullptr;

ModMenuModule::ForceControlsCheat::ForceControlsCheat() : ModMenuModule::CheatBase("") {
	assert(m_instance == nullptr && "ForceControlsCheat instance already exists");
	m_instance = this;
}

ModMenuModule::ForceControlsCheat::~ForceControlsCheat()
{
	m_instance = nullptr;
}

ModMenuModule::ForceControlsCheat* ModMenuModule::ForceControlsCheat::GetInstance()
{
	assert(m_instance && "ForceControlsCheat not initialized!");
	return m_instance;
}

ModMenuModule::ForceControlsCheat::ControlIndex ModMenuModule::ForceControlsCheat::GetControlIndex(Game::KEYBOARD_STATE control)
{
	size_t shiftedControl = static_cast<size_t>(control) & (1U << CONTROLS_COUNT) - 1;
	if (shiftedControl == 0) return -1;

	for (size_t i = 0; i < CONTROLS_COUNT; i++) {
		if (shiftedControl == 1) return i;
		shiftedControl = shiftedControl >> 1;
	}

	return -1;
}

bool ModMenuModule::ForceControlsCheat::CheckUsesController(ControlIndex controlIndex)
{
	assert(controlIndex < CONTROLS_COUNT);

	Game::Controls* controls = Game::Memory::GetControls();
	unsigned int controlKey = (reinterpret_cast<unsigned int*>(controls))[controlIndex];
	return controlKey >= 0xE0 && controlKey <= 0xE3 || controlKey == 0x00 || controlKey == 0x01;
}

bool ModMenuModule::ForceControlsCheat::CheckGameReadyToForce(bool ignoreGamePause)
{
	Game::Game* game = Game::Memory::GetGame();
	if (!game) return false;

	if (game->gameStatus != Game::GAME_RUN) {
		if (!ignoreGamePause && game->gameStatus == Game::GAME_PAUSED) {
			return false;
		}
	}

	Game::Keyboard* keyboard = Game::Memory::GetKeyboard();
	if (!keyboard) return false;

	return keyboard->replayStatus == Game::REPLAY_STATUS_NOT_PLAYING;
}

bool ModMenuModule::ForceControlsCheat::IsControlFree(ControlIndex controlIndex) const
{
	assert(controlIndex < CONTROLS_COUNT);

	const Control& controlState = m_controls.at(controlIndex);
	return controlState.handle == -1;
}

ModMenuModule::ForceControlsCheat::ControlHandle ModMenuModule::ForceControlsCheat::CreateControlHandle(ControlIndex controlIndex)
{
	assert(controlIndex < CONTROLS_COUNT);

	Control& controlState = m_controls.at(controlIndex);
	if (controlState.handle != -1) {
		spdlog::error("Control {} already has a handle", controlIndex);
		return static_cast<ControlHandle>(-1);
	}

	if (m_nextControlHandle == -2) m_nextControlHandle = 1;
	ControlHandle newHandle = m_nextControlHandle++;
	controlState.handle = newHandle;
	m_controlHandles[newHandle] = controlIndex;

	SetEnabled(true);
	return newHandle;
}

void ModMenuModule::ForceControlsCheat::FreeControlHandle(ControlHandle handle)
{
	if (m_controlHandles.find(handle) == m_controlHandles.end()) {
		spdlog::error("Control handle {} does not exist", handle);
		return;
	}

	ControlIndex controlIndex = m_controlHandles.at(handle);
	Control& controlState = m_controls.at(controlIndex);
	controlState.handle = -1;
	controlState.nextState = ForceControlState::Unmodified;
	m_controlHandles.erase(handle);
}

void ModMenuModule::ForceControlsCheat::SetControlState(ControlHandle controlHandle, ForceControlState state)
{
	if (m_controlHandles.find(controlHandle) == m_controlHandles.end()) {
		spdlog::error("Control handle {} does not exist", controlHandle);
		return;
	}

	ControlIndex controlIndex = m_controlHandles.at(controlHandle);
	Control& controlState = m_controls.at(controlIndex);
	controlState.nextState = state;

	Game::Keyboard* keyboard = Game::Memory::GetKeyboard();
	if (state != ForceControlState::Unmodified && controlState.state == ForceControlState::Unmodified) {
		controlState.unmodifiedIsDown = (keyboard->state & (1U << controlIndex)) != 0;
	}
}

ModMenuModule::ForceControlState ModMenuModule::ForceControlsCheat::GetControlState(ControlHandle controlHandle) const
{
	if (m_controlHandles.find(controlHandle) == m_controlHandles.end()) {
		spdlog::error("Control handle {} does not exist", controlHandle);
		return ForceControlState::Unmodified;
	}

	ControlIndex controlIndex = m_controlHandles.at(controlHandle);
	const Control& controlState = m_controls.at(controlIndex);
	return controlState.nextState;
}

void ModMenuModule::ForceControlsCheat::OnFirstEnable()
{

}

void ModMenuModule::ForceControlsCheat::OnEnable()
{
	AddEventListener<GameStartEvent>(&ForceControlsCheat::OnGameStart);
	AddEventListener<GameEndEvent>(&ForceControlsCheat::OnGameEnd);
	AddEventListener<GamePauseEvent>(&ForceControlsCheat::OnGamePause);
	AddEventListener<GameUnpauseEvent>(&ForceControlsCheat::OnGameUnpause);
	AddEventListener<DrawFrameEvent>(&ForceControlsCheat::OnDrawFrame);

	if (CheckGameReadyToForce()) Start();
}

void ModMenuModule::ForceControlsCheat::OnDisable()
{
	RemoveEventListener<GameStartEvent>();
	RemoveEventListener<GameEndEvent>();
	RemoveEventListener<GamePauseEvent>();
	RemoveEventListener<GameUnpauseEvent>();
	RemoveEventListener<DrawFrameEvent>();

	End();

	if (CheckCheatNeeded()) {
		spdlog::warn("ForceControlsCheat: Cheat is being unsafely disabled.");
		m_controlHandles.clear();
		m_controls.fill({});
	}
}

void ModMenuModule::ForceControlsCheat::OnKeyboardGetData(KeyboardGetDataEvent& event)
{
	PreProcessEvent(event);
	if (!event.IsReadyToEmulate()) return;

	for (size_t i = 0; i < CONTROLS_COUNT; i++) {
		if (ProcessControl(event, i)) break;
	}
}

void ModMenuModule::ForceControlsCheat::OnGameStart(GameStartEvent& event)
{
	if (CheckGameReadyToForce()) Start();
}

void ModMenuModule::ForceControlsCheat::OnGameEnd(GameEndEvent& event)
{
	End();
}

void ModMenuModule::ForceControlsCheat::OnGamePause(GamePauseEvent& event)
{
	End();
}

void ModMenuModule::ForceControlsCheat::OnGameUnpause(GameUnpauseEvent& event)
{
	if (CheckGameReadyToForce(true)) Start();
}

void ModMenuModule::ForceControlsCheat::OnDrawFrame(DrawFrameEvent& event)
{
	if (!CheckCheatNeeded()) {
		SetEnabled(false);
	}
}

void ModMenuModule::ForceControlsCheat::Start()
{
	if (m_started) return;
	m_started = true;

	AddEventListener<KeyboardGetDataEvent>(&ForceControlsCheat::OnKeyboardGetData);
}

void ModMenuModule::ForceControlsCheat::End()
{
	if (!m_started) return;
	m_started = false;

	RemoveEventListener<KeyboardGetDataEvent>();
}

bool ModMenuModule::ForceControlsCheat::CheckCheatNeeded() const
{
	if (!m_controlHandles.empty()) return true;

	for (const Control& controlState : m_controls) {
		if (controlState.state != ForceControlState::Unmodified) {
			return true;
		}
	}

	return false;
}

void ModMenuModule::ForceControlsCheat::PreProcessEvent(KeyboardGetDataEvent& event)
{
	if (event.IsReadyToEmulate()) return;
	Game::Controls* controls = Game::Memory::GetControls();

	for (size_t i = 0; i < CONTROLS_COUNT; i++) {
		Game::KeyCode controlKeyCode = (reinterpret_cast<Game::KeyCode*>(controls))[i];
		if (event.GetKeyCode() != controlKeyCode) continue;
		Control& controlState = m_controls.at(i);

		const ForceControlState& state = controlState.state;
		const ForceControlState& nextState = controlState.nextState;

		bool isModified = state != ForceControlState::Unmodified || nextState != ForceControlState::Unmodified;

		if (isModified) {
			controlState.unmodifiedIsDown = event.GetModifiedIsDown();
			event.Drop();
		}
	}
}

bool ModMenuModule::ForceControlsCheat::ProcessControl(KeyboardGetDataEvent& event, ControlIndex controlIndex)
{
	Control& control = m_controls.at(controlIndex);
	ForceControlState& state = control.state;
	ForceControlState& nextState = control.nextState;
	bool currentNextMatch = state == nextState;

	Game::Controls* controls = Game::Memory::GetControls();
	Game::KeyCode controlKeyCode = (reinterpret_cast<Game::KeyCode*>(controls))[controlIndex];
	Game::Keyboard* keyboard = Game::Memory::GetKeyboard();
	Game::KEYBOARD_STATE controlMask = static_cast<Game::KEYBOARD_STATE>(1U << controlIndex);
	bool isDown = (keyboard->state & controlMask) != 0;

	switch (nextState) {
	case ForceControlState::Unmodified:
		if (currentNextMatch) return false;
		state = ForceControlState::Unmodified;
		if (isDown != control.unmodifiedIsDown) {
			event.EmulateKey(controlKeyCode, control.unmodifiedIsDown);
			return true;
		}
		return false;
	case ForceControlState::ForceDown:
		state = ForceControlState::ForceDown;
		if (!isDown) {
			if (currentNextMatch) {
				spdlog::warn("ForceControlsCheat: Control {} is forced down, but the keyboard state was modified", controlIndex);
				control.unmodifiedIsDown = false;
			}
			event.EmulateKey(controlKeyCode, true);
			return true;
		}
		return false;
	case ForceControlState::ForceUp:
		state = ForceControlState::ForceUp;
		if (isDown) {
			if (currentNextMatch) {
				spdlog::warn("ForceControlsCheat: Control {} is forced up, but the keyboard state was modified", controlIndex);
				control.unmodifiedIsDown = true;
			}
			event.EmulateKey(controlKeyCode, false);
			return true;
		}
		return false;
	}

	return false;
}

REGISTER_CHEAT(ForceControlsCheat)

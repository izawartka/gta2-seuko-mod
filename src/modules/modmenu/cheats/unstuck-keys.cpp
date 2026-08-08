#include "unstuck-keys.h"
#include "../cheat-registry.h"

static constexpr unsigned int UNSTUCK_KEYS_KEYBOARD_PRIORITY = 100;

ModMenuModule::UnstuckKeysCheat* ModMenuModule::UnstuckKeysCheat::m_instance = nullptr;

ModMenuModule::UnstuckKeysCheat::UnstuckKeysCheat() : ModMenuModule::CheatBase("") {
	assert(m_instance == nullptr && "UnstuckKeysCheat instance already exists");
	m_instance = this;
}

ModMenuModule::UnstuckKeysCheat::~UnstuckKeysCheat()
{
	m_instance = nullptr;
}

ModMenuModule::UnstuckKeysCheat* ModMenuModule::UnstuckKeysCheat::GetInstance()
{
	assert(m_instance && "UnstuckKeysCheat not initialized!");
	return m_instance;
}

void ModMenuModule::UnstuckKeysCheat::OnFirstEnable()
{
	
}

void ModMenuModule::UnstuckKeysCheat::OnEnable()
{
	AddEventListener<WindowFocusChangeEvent>(&ModMenuModule::UnstuckKeysCheat::OnWindowFocusChange);
	AddEventListener<GameStartEvent>(&ModMenuModule::UnstuckKeysCheat::OnGameStart);
	AddEventListener<KeyboardGetDataEvent>(&ModMenuModule::UnstuckKeysCheat::OnKeyboardGetData, false, UNSTUCK_KEYS_KEYBOARD_PRIORITY);
}

void ModMenuModule::UnstuckKeysCheat::OnDisable()
{
	RemoveEventListener<WindowFocusChangeEvent>();
	RemoveEventListener<GameStartEvent>();
	RemoveEventListener<KeyboardGetDataEvent>();
	StopUnstuck();
}

void ModMenuModule::UnstuckKeysCheat::OnWindowFocusChange(WindowFocusChangeEvent& event)
{
	if (!event.GetIsFocused()) return;
	if (m_heldKeys.empty()) return;

	spdlog::debug("UnstuckKeysCheat: Window regained focus, unstucking {} held key(s)", m_heldKeys.size());
	m_stuckKeys.insert(m_heldKeys.begin(), m_heldKeys.end());
	m_unstuckInProgress = true;
}

void ModMenuModule::UnstuckKeysCheat::OnGameStart(GameStartEvent& event)
{
	if (m_heldKeys.empty()) return;

	spdlog::debug("UnstuckKeysCheat: Game restarted, unstucking {} held key(s)", m_heldKeys.size());
	m_stuckKeys.insert(m_heldKeys.begin(), m_heldKeys.end());
	m_unstuckInProgress = true;
}

void ModMenuModule::UnstuckKeysCheat::OnKeyboardGetData(KeyboardGetDataEvent& event)
{
	if (event.IsReadyToEmulate()) {
		if (!m_unstuckInProgress) return;
		if (m_stuckKeys.empty()) {
			StopUnstuck();
			return;
		}

		Game::KeyCode stuckKey = *m_stuckKeys.begin();
		event.EmulateKey(stuckKey, false);
	}

	if (event.GetModifiedDataCount() != 0) {
		Game::KeyCode keyCode = event.GetModifiedKeyCode();
		bool isDown = event.GetModifiedIsDown();

		if (isDown) {
			m_heldKeys.insert(keyCode);
			return;
		}

		m_heldKeys.erase(keyCode);
		m_stuckKeys.erase(keyCode);

		if (m_stuckKeys.empty() && m_unstuckInProgress) {
			StopUnstuck();
		}
	}
}

void ModMenuModule::UnstuckKeysCheat::StopUnstuck()
{
	spdlog::debug("UnstuckKeysCheat: Stopping unstuck process", m_stuckKeys.size());
	m_stuckKeys.clear();
	m_unstuckInProgress = false;
}

REGISTER_CHEAT(UnstuckKeysCheat)

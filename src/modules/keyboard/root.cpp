#include "root.h"
#include "events/key-down.h"
#include "events/key-up.h"

static constexpr unsigned int KEYBOARD_EVENT_PRIORITY = 100 * 1000;

KeyboardModule::RootModule* KeyboardModule::RootModule::m_instance = nullptr;

KeyboardModule::RootModule::RootModule() {
	assert(m_instance == nullptr && "KeyboardModule::RootModule instance already exists!");
	m_instance = this;
	spdlog::info("KeyboardModule::RootModule module initialized.");
}

KeyboardModule::RootModule::~RootModule()
{
	m_instance = nullptr;
}

KeyboardModule::RootModule* KeyboardModule::RootModule::GetInstance()
{
	return m_instance;
}

bool KeyboardModule::RootModule::Attach()
{
	EventListenerSupport::AddEventListener<KeyboardGetDataEvent>(&RootModule::OnKeyboardGetData, false, KEYBOARD_EVENT_PRIORITY);
	spdlog::info("KeyboardModule::RootModule module attached.");
	return true;
}

void KeyboardModule::RootModule::Detach()
{
	EventListenerSupport::RemoveEventListener<KeyboardGetDataEvent>();
	spdlog::info("KeyboardModule::RootModule module detached.");
}

bool KeyboardModule::RootModule::IsKeyDown(Game::KeyCode keyCode) const
{
	unsigned char keyIndex = static_cast<unsigned char>(keyCode);
	return m_keys[keyIndex];
}

bool KeyboardModule::RootModule::IsCapsLockOn() const
{
	return m_isCapsLockOn;
}

void KeyboardModule::RootModule::OnKeyboardGetData(KeyboardGetDataEvent& event)
{
	if (event.GetModifiedDataCount() == 0) return;

	bool isDown = event.GetModifiedIsDown();
	Game::KeyCode keyCode = event.GetModifiedKeyCode();

	UpdateCapsLock();

	bool prevIsDown = SetKeyState(keyCode, isDown);
	if (isDown == prevIsDown) {
		spdlog::warn(
			"KeyboardModule: Recieved key {} update event, but its state is unchanged ({})",
			static_cast<unsigned char>(keyCode),
			isDown ? "down" : "up"
		);
	}

	Core::EventManager* eventManager = Core::EventManager::GetInstance();
	if (isDown) {
		KeyDownEvent keyEvent(
			keyCode, 
			IsKeyDown(Game::KeyCode::DIK_LSHIFT) || IsKeyDown(Game::KeyCode::DIK_RSHIFT),
			IsKeyDown(Game::KeyCode::DIK_LCONTROL) || IsKeyDown(Game::KeyCode::DIK_RCONTROL),
			IsKeyDown(Game::KeyCode::DIK_LMENU) || IsKeyDown(Game::KeyCode::DIK_RMENU),
			m_isCapsLockOn
		);
		eventManager->Dispatch(keyEvent);
		if (keyEvent.IsCancelled()) event.Drop();
	} else {
		KeyUpEvent keyEvent(
			keyCode,
			IsKeyDown(Game::KeyCode::DIK_LSHIFT) || IsKeyDown(Game::KeyCode::DIK_RSHIFT),
			IsKeyDown(Game::KeyCode::DIK_LCONTROL) || IsKeyDown(Game::KeyCode::DIK_RCONTROL),
			IsKeyDown(Game::KeyCode::DIK_LMENU) || IsKeyDown(Game::KeyCode::DIK_RMENU),
			m_isCapsLockOn
		);
		eventManager->Dispatch(keyEvent);
		if (keyEvent.IsCancelled()) event.Drop();
	}
}

bool KeyboardModule::RootModule::SetKeyState(Game::KeyCode keyCode, bool isDown)
{
	unsigned char keyIndex = static_cast<unsigned char>(keyCode);
	bool prevState = m_keys[keyIndex];
	m_keys[keyIndex] = isDown;

	return prevState;
}

void KeyboardModule::RootModule::UpdateCapsLock()
{
	m_isCapsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

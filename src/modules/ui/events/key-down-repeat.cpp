#include "key-down-repeat.h"
#include "update-ui.h"

static constexpr int KEY_DOWN_REPEAT_DELAY = 20; // frames
static constexpr int KEY_DOWN_REPEAT_INTERVAL = 3; // frames

static std::optional<KeyBindingModule::Key> heldKey = std::nullopt;
static int holdTime = 0;
static Core::EventListenerId keyDownListenerId = 0;
static Core::EventListenerId keyUpListenerId = 0;
static Core::EventListenerId preUpdateUiListenerId = 0;

static void OnKeyDown(KeyboardModule::KeyDownEvent& event)
{
	heldKey = KeyBindingModule::Key::FromKeyboardEvent(event);
	holdTime = 1;

	KeyboardModule::RootModule* keyboardRoot = KeyboardModule::RootModule::GetInstance();
	bool isCapsLockOn = keyboardRoot->IsCapsLockOn();
	UiModule::KeyDownRepeatEvent repeatEvent(heldKey->keyCode, heldKey->shift, heldKey->ctrl, heldKey->alt, isCapsLockOn);
	Core::EventManager::GetInstance()->Dispatch(repeatEvent);
	if (repeatEvent.IsCancelled()) event.Cancel();
}

static void OnKeyUp(KeyboardModule::KeyUpEvent& event)
{
	heldKey = std::nullopt;
	holdTime = 0;
}

static void OnPreUpdateUI(UiModule::PreUpdateUIEvent& event)
{
	if (holdTime == 0) return;

	int repeatTime = holdTime - KEY_DOWN_REPEAT_DELAY;
	if (repeatTime >= 0 && repeatTime % KEY_DOWN_REPEAT_INTERVAL == 0) {
		KeyboardModule::RootModule* keyboardRoot = KeyboardModule::RootModule::GetInstance();
		bool isCapsLockOn = keyboardRoot->IsCapsLockOn();
		UiModule::KeyDownRepeatEvent event(heldKey->keyCode, heldKey->shift, heldKey->ctrl, heldKey->alt, isCapsLockOn);
		Core::EventManager::GetInstance()->Dispatch(event);
	}

	holdTime++;
}

bool UiModule::KeyDownRepeatEvent::Init()
{
	Core::EventManager* eventManager = Core::EventManager::GetInstance();

	keyDownListenerId = eventManager->AddListener<KeyboardModule::KeyDownEvent>(OnKeyDown, false, UI_KEYBOARD_EVENT_PRIORITY);
	keyUpListenerId = eventManager->AddListener<KeyboardModule::KeyUpEvent>(OnKeyUp, false, UI_KEYBOARD_EVENT_PRIORITY);
	preUpdateUiListenerId = eventManager->AddListener<PreUpdateUIEvent>(OnPreUpdateUI);

	return true;
}

void UiModule::KeyDownRepeatEvent::Deinit()
{
	Core::EventManager* eventManager = Core::EventManager::GetInstance();

	eventManager->RemoveListener<KeyboardModule::KeyDownEvent>(keyDownListenerId);
	keyDownListenerId = 0;
	eventManager->RemoveListener<KeyboardModule::KeyUpEvent>(keyUpListenerId);
	keyUpListenerId = 0;
	eventManager->RemoveListener<PreUpdateUIEvent>(preUpdateUiListenerId);
	preUpdateUiListenerId = 0;
}

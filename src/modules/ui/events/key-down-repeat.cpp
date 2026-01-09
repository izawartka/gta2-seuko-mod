#include "key-down-repeat.h"
#include "update-ui.h"

static constexpr int KEY_DOWN_REPEAT_DELAY = 20; // frames
static constexpr int KEY_DOWN_REPEAT_INTERVAL = 3; // frames

static std::optional<KeyBindingModule::Key> heldKey = std::nullopt;
static int holdTime = 0;
static Core::EventListenerId keyDownListenerId = 0;
static Core::EventListenerId keyUpListenerId = 0;
static Core::EventListenerId preUpdateUiListenerId = 0;

static void OnKeyDown(KeyDownEvent& event)
{
	heldKey = KeyBindingModule::Key::FromKeyboardEvent(event);
	holdTime = 1;
}

static void OnKeyUp(KeyUpEvent& event)
{
	heldKey = std::nullopt;
	holdTime = 0;
}

static void OnPreUpdateUI(UiModule::PreUpdateUIEvent& event)
{
	if (holdTime == 0) return;

	int repeatTime = holdTime - KEY_DOWN_REPEAT_DELAY;
	if (holdTime == 1 || repeatTime >= 0 && repeatTime % KEY_DOWN_REPEAT_INTERVAL == 0) {
		UiModule::KeyDownRepeatEvent event(heldKey->keyCode, heldKey->shift, heldKey->ctrl, heldKey->alt);
		Core::EventManager::GetInstance()->Dispatch(event);
	}

	holdTime++;
}

bool UiModule::KeyDownRepeatEvent::Init()
{
	Core::EventManager* eventManager = Core::EventManager::GetInstance();

	keyDownListenerId = eventManager->AddListener<KeyDownEvent>(OnKeyDown);
	keyUpListenerId = eventManager->AddListener<KeyUpEvent>(OnKeyUp);
	preUpdateUiListenerId = eventManager->AddListener<PreUpdateUIEvent>(OnPreUpdateUI);

	return true;
}

void UiModule::KeyDownRepeatEvent::Deinit()
{
	Core::EventManager* eventManager = Core::EventManager::GetInstance();

	eventManager->RemoveListener<KeyDownEvent>(keyDownListenerId);
	keyDownListenerId = 0;
	eventManager->RemoveListener<KeyUpEvent>(keyUpListenerId);
	keyUpListenerId = 0;
	eventManager->RemoveListener<PreUpdateUIEvent>(preUpdateUiListenerId);
	preUpdateUiListenerId = 0;
}

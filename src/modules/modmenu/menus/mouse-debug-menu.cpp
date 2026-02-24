#include "mouse-debug-menu.h"
#include "../root.h"
#include "../../../converters/yes-no.h"

ModMenuModule::MouseDebugMenu::MouseDebugMenu()
{

}

ModMenuModule::MouseDebugMenu::~MouseDebugMenu()
{

}

bool ModMenuModule::MouseDebugMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Mouse debug menu#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	auto subMouseMoveText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_subMouseMoveController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		subMouseMoveText,
		UiModule::SelectOptionList<bool>{ false, true },
		false,
		UiModule::SelectControllerOptions{ L"Subscribe MouseMoveEvent: #", L"#" }
	);
	m_subMouseMoveController->SetConverter<YesNoConverter>();
	m_subMouseMoveController->SetSaveCallback([this](bool newValue) {
		SetEventListener<MouseModule::MouseMoveEvent>(&MouseDebugMenu::OnMouseMove, newValue);
	});

	auto subMouseLockedMoveText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_subMouseLockedMoveController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		subMouseLockedMoveText,
		UiModule::SelectOptionList<bool>{ false, true },
		false,
		UiModule::SelectControllerOptions{ L"Subscribe MouseLockedMoveEvent: #", L"#" }
	);
	m_subMouseLockedMoveController->SetConverter<YesNoConverter>();
	m_subMouseLockedMoveController->SetSaveCallback([this](bool newValue) {
		SetEventListener<MouseModule::MouseLockedMoveEvent>(&MouseDebugMenu::OnMouseLockedMove, newValue);
	});

	auto subMouseButtonDownText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_subMouseButtonDownController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		subMouseButtonDownText,
		UiModule::SelectOptionList<bool>{ false, true },
		false,
		UiModule::SelectControllerOptions{ L"Subscribe MouseButtonDownEvent: #", L"#" }
	);
	m_subMouseButtonDownController->SetConverter<YesNoConverter>();
	m_subMouseButtonDownController->SetSaveCallback([this](bool newValue) {
		SetEventListener<MouseModule::MouseButtonDownEvent>(&MouseDebugMenu::OnMouseButtonDown, newValue);
	});

	auto subMouseButtonUpText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_subMouseButtonUpController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		subMouseButtonUpText,
		UiModule::SelectOptionList<bool>{ false, true },
		false,
		UiModule::SelectControllerOptions{ L"Subscribe MouseButtonUpEvent: #", L"#" }
	);
	m_subMouseButtonUpController->SetConverter<YesNoConverter>();
	m_subMouseButtonUpController->SetSaveCallback([this](bool newValue) {
		SetEventListener<MouseModule::MouseButtonUpEvent>(&MouseDebugMenu::OnMouseButtonUp, newValue);
	});

	auto subMouseEnterText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_subMouseEnterController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		subMouseEnterText,
		UiModule::SelectOptionList<bool>{ false, true },
		false,
		UiModule::SelectControllerOptions{ L"Subscribe MouseEnterEvent: #", L"#" }
	);
	m_subMouseEnterController->SetConverter<YesNoConverter>();
	m_subMouseEnterController->SetSaveCallback([this](bool newValue) {
		SetEventListener<MouseModule::MouseEnterEvent>(&MouseDebugMenu::OnMouseEnter, newValue);
	});

	auto subMouseLeaveText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_subMouseLeaveController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		subMouseLeaveText,
		UiModule::SelectOptionList<bool>{ false, true },
		false,
		UiModule::SelectControllerOptions{ L"Subscribe MouseLeaveEvent: #", L"#" }
	);
	m_subMouseLeaveController->SetConverter<YesNoConverter>();
	m_subMouseLeaveController->SetSaveCallback([this](bool newValue) {
		SetEventListener<MouseModule::MouseLeaveEvent>(&MouseDebugMenu::OnMouseLeave, newValue);
	});

	auto lockMouseText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_lockMouseController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		lockMouseText,
		[]() -> bool {
			MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
			return mouseManager->IsLocked();
		},
		UiModule::VarTextSelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Lock mouse: #", L"#" }
	);
	m_lockMouseController->SetConverter<YesNoConverter>();
	m_lockMouseController->SetCustomSaveCallback([this](bool newValue) {
		MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
		mouseManager->SetLocked(newValue);
	});

	auto invisibleMouseText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_invisibleMouseController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		invisibleMouseText,
		[]() -> bool {
			MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
			return mouseManager->IsInvisible();
		},
		UiModule::VarTextSelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Mouse invisible: #", L"#" }
	);
	m_invisibleMouseController->SetConverter<YesNoConverter>();
	m_invisibleMouseController->SetCustomSaveCallback([this](bool newValue) {
		MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
		mouseManager->SetInvisible(newValue);
	});

	m_posXText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_posYText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_leftButtonText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_rightButtonText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_middleButtonText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_inWindowText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::MouseDebugMenu::OnShow()
{
	AddEventListener<UiModule::PreUpdateUIEvent>(&MouseDebugMenu::OnPreUpdateUI);
}

void ModMenuModule::MouseDebugMenu::OnHide()
{
	RemoveAllEventListeners();
	m_subMouseMoveController->SetValue(false);
	m_subMouseLockedMoveController->SetValue(false);
	m_subMouseButtonDownController->SetValue(false);
	m_subMouseButtonUpController->SetValue(false);
	m_subMouseEnterController->SetValue(false);
	m_subMouseLeaveController->SetValue(false);
}

void ModMenuModule::MouseDebugMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::MouseDebugMenu::OnPreUpdateUI(UiModule::PreUpdateUIEvent& event)
{
	UpdateState();
}

void ModMenuModule::MouseDebugMenu::OnMouseMove(MouseModule::MouseMoveEvent& event)
{
	spdlog::debug("Mouse moved to position ({}, {})", event.GetPosition().x, event.GetPosition().y);
}

void ModMenuModule::MouseDebugMenu::OnMouseLockedMove(MouseModule::MouseLockedMoveEvent& event)
{
	spdlog::debug("Mouse moved (locked) with delta ({}, {})", event.GetDelta().x, event.GetDelta().y);
}

void ModMenuModule::MouseDebugMenu::OnMouseButtonDown(MouseModule::MouseButtonDownEvent& event)
{
	spdlog::debug("Mouse button {} down", ButtonNameToString(event.GetButton()));
}

void ModMenuModule::MouseDebugMenu::OnMouseButtonUp(MouseModule::MouseButtonUpEvent& event)
{
	spdlog::debug("Mouse button {} up", ButtonNameToString(event.GetButton()));
}

void ModMenuModule::MouseDebugMenu::OnMouseEnter(MouseModule::MouseEnterEvent& event)
{
	spdlog::debug("Mouse entered window");
}

void ModMenuModule::MouseDebugMenu::OnMouseLeave(MouseModule::MouseLeaveEvent& event)
{
	spdlog::debug("Mouse left window");
}

void ModMenuModule::MouseDebugMenu::UpdateState()
{
	MouseModule::MouseState state = MouseModule::MouseManager::FetchMouseState();
	m_posXText->SetText(L"X: " + std::to_wstring(state.position.x));
	m_posYText->SetText(L"Y: " + std::to_wstring(state.position.y));
	m_leftButtonText->SetText(L"Left button: " + std::wstring(state.leftButtonDown ? L"Down" : L"Up"));
	m_rightButtonText->SetText(L"Right button: " + std::wstring(state.rightButtonDown ? L"Down" : L"Up"));
	m_middleButtonText->SetText(L"Middle button: " + std::wstring(state.middleButtonDown ? L"Down" : L"Up"));
	m_inWindowText->SetText(L"In window: " + std::wstring(state.isInWindow ? L"Yes" : L"No"));
}

std::string ModMenuModule::MouseDebugMenu::ButtonNameToString(MouseModule::MouseButton button)
{
	switch (button) {
	case MouseModule::MouseButton::Left:
		return "Left";
	case MouseModule::MouseButton::Right:
		return "Right";
	case MouseModule::MouseButton::Middle:
		return "Middle";
	default:
		return "Unknown";
	}
}

#include "mouse-control-menu.h"
#include "../root.h"
#include "../cheats/mouse-control.h"
#include "../../../converters/enabled-disabled.h"
#include "../../../converters/mouse-control-cheat-mode.h"

static constexpr float ROTATE_MODE_SENSITIVITY_MULTIPLIER = 0.001f;

ModMenuModule::MouseControlMenu::MouseControlMenu()
{

}

ModMenuModule::MouseControlMenu::~MouseControlMenu()
{

}

bool ModMenuModule::MouseControlMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Mouse control options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// mouse control cheat
	UiModule::Text* mouseControlCheatText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_mouseControlCheatController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		mouseControlCheatText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Mouse control: #", L"#" }
	);
	m_mouseControlCheatController->SetConverter<EnabledDisabledConverter>();
	m_mouseControlCheatController->SetSaveCallback([mouseControlCheat](bool newValue) {
		mouseControlCheat->SetEnabled(newValue);
	});

	m_cheatItemsCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);
	m_cheatItemsGroupId = m_menuController->RegisterGroup();

	m_rotateModeItemsCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);
	m_rotateModeItemsGroupId = m_menuController->RegisterGroup();
	
	UpdateAttachedItems();
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::MouseControlMenu::Detach()
{
	DetachCheatMenuItems();
	DetachRotateModeMenuItems();
	DestroyMenu();
}

void ModMenuModule::MouseControlMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&MouseControlMenu::OnCheatStateChange);
	AddEventListener<ModMenuModule::CheatOptionsUpdateEvent<MouseControlCheat>>(&MouseControlMenu::OnCheatOptionsUpdate);
	UpdateAttachedItems();
}

void ModMenuModule::MouseControlMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
	RemoveEventListener<ModMenuModule::CheatOptionsUpdateEvent<MouseControlCheat>>();
}

void ModMenuModule::MouseControlMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::MouseControlMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(MouseControlCheat)) {
		m_mouseControlCheatController->SetValue(event.IsEnabled());

		if (event.IsEnabled()) {
			AttachCheatMenuItems();
			UpdateRotateModeMenuItemsAttached();
		}
		else {
			DetachCheatMenuItems();
			DetachRotateModeMenuItems();
		}
	}
}

void ModMenuModule::MouseControlMenu::OnCheatOptionsUpdate(CheatOptionsUpdateEvent<MouseControlCheat>& event)
{
	UpdateRotateModeMenuItemsAttached();
}

void ModMenuModule::MouseControlMenu::AttachCheatMenuItems()
{

	if (m_cheatItemsAttached) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	UiModule::VertCont* container = m_cheatItemsCont;

	m_menuController->SetCurrentGroupId(m_cheatItemsGroupId);

	// auto mode
	UiModule::Text* autoModeText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto autoModeController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		autoModeText,
		[mouseControlCheat]() {
			return mouseControlCheat->GetOptions().autoMode;
		},
		std::vector<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Auto mode: #", L"#" }
	);
	autoModeController->SetConverter<EnabledDisabledConverter>();
	autoModeController->SetCustomSaveCallback([mouseControlCheat, this](bool newValue) {
		MouseControlCheatOptions options = mouseControlCheat->GetOptions();
		options.autoMode = newValue;
		mouseControlCheat->SetOptions(options);
	});

	// mode
	UiModule::Text* modeText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto modeController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<MouseControlCheatMode, MouseControlCheatMode>>(
		modeText,
		[mouseControlCheat]() {
			return mouseControlCheat->GetOptions().mode;
		},
		MouseControlCheat::GetAllMouseControlCheatModes(),
		UiModule::VarTextSelectControllerOptions{ L"Mode: #", L"#" }
	);
	modeController->SetConverter<MouseControlCheatModeConverter>();
	modeController->SetCustomSaveCallback([mouseControlCheat, this](MouseControlCheatMode newValue) {
		MouseControlCheatOptions options = mouseControlCheat->GetOptions();
		options.mode = newValue;
		options.autoMode = false;
		mouseControlCheat->SetOptions(options);
	});

	m_cheatItemsAttached = true;
}

void ModMenuModule::MouseControlMenu::DetachCheatMenuItems()
{
	if (!m_cheatItemsAttached) return;

	m_menuController->DeleteGroupItems(m_cheatItemsGroupId);
	m_cheatItemsAttached = false;
}

void ModMenuModule::MouseControlMenu::AttachRotateModeMenuItems()
{
	if (m_rotateModeItemsAttached) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	UiModule::VertCont* container = m_rotateModeItemsCont;

	m_menuController->SetCurrentGroupId(m_rotateModeItemsGroupId);

	// rotate mode sensitivity
	UiModule::Text* rotateModeSensitivityText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto rotateModeSensitivityController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<unsigned int, unsigned int>>(
		rotateModeSensitivityText,
		[mouseControlCheat]() {
			float sensitivity = mouseControlCheat->GetOptions().rotateModeSensitivity;
			float sensitivityValue = sensitivity / ROTATE_MODE_SENSITIVITY_MULTIPLIER;
			return static_cast<unsigned int>(sensitivityValue + 0.5f);
		},
		UiModule::VarTextEditableControllerOptions{ L"Sensitivity: #", L"%#" }
	);
	rotateModeSensitivityController->SetCustomSaveCallback([mouseControlCheat](unsigned int newValue) {
		MouseControlCheatOptions options = mouseControlCheat->GetOptions();
		float sensitivityValue = static_cast<float>(newValue) * ROTATE_MODE_SENSITIVITY_MULTIPLIER;
		options.rotateModeSensitivity = sensitivityValue;
		mouseControlCheat->SetOptions(options);
	});
	rotateModeSensitivityController->SetClampCallback([](unsigned int newValue) {
		if (newValue < 1U) return 1U;
		if (newValue > 100U) return 100U;
		return newValue;
	});

	m_rotateModeItemsAttached = true;
}

void ModMenuModule::MouseControlMenu::DetachRotateModeMenuItems()
{
	if (!m_rotateModeItemsAttached) return;

	m_menuController->DeleteGroupItems(m_rotateModeItemsGroupId);
	m_rotateModeItemsAttached = false;
}

void ModMenuModule::MouseControlMenu::UpdateAttachedItems()
{
	bool mouseControlCheatEnabled = MouseControlCheat::GetInstance()->IsEnabled();
	m_mouseControlCheatController->SetValue(mouseControlCheatEnabled);

	if (mouseControlCheatEnabled) {
		AttachCheatMenuItems();
		UpdateRotateModeMenuItemsAttached();
	}
	else {
		DetachCheatMenuItems();
		DetachRotateModeMenuItems();
	}
}

void ModMenuModule::MouseControlMenu::UpdateRotateModeMenuItemsAttached()
{
	MouseControlCheat* mouseControlCheat = MouseControlCheat::GetInstance();

	if (!mouseControlCheat->IsEnabled()) {
		DetachRotateModeMenuItems();
		return;
	}

	MouseControlCheatOptions options = mouseControlCheat->GetOptions();
	bool isAutoMode = options.autoMode;
	bool isRotateMode = options.mode == MouseControlCheatMode::Rotate;

	if (isAutoMode || isRotateMode) {
		AttachRotateModeMenuItems();
	}
	else {
		DetachRotateModeMenuItems();
	}
}

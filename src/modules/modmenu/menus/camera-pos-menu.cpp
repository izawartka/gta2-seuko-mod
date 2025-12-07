#include "camera-pos-menu.h"
#include "../root.h"
#include "../cheats/camera/camera-pos.h"
#include "../../../converters/enabled-disabled.h"
#include "../../../converters/camera-pos-cheat-mode.h"
#include "../../../converters/scrf.h"

ModMenuModule::CameraPosMenu::CameraPosMenu()
{

}

ModMenuModule::CameraPosMenu::~CameraPosMenu()
{

}

bool ModMenuModule::CameraPosMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Camera position#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	CameraPosCheat* cameraPosCheat = GetCheat<CameraPosCheat>();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// camera pos cheat
	UiModule::Text* cameraPosCheatText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_cameraPosCheatController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		cameraPosCheatText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Override position: #", L"#" }
	);
	m_cameraPosCheatController->SetConverter<EnabledDisabledConverter>();
	m_cameraPosCheatController->SetSaveCallback([cameraPosCheat](bool newValue) {
		if(newValue) cameraPosCheat->SetEnabled(true);
		else cameraPosCheat->SnapAndDisable();
	});

	m_cheatItemsOuterCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);

	UpdateCheatStates();
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::CameraPosMenu::Detach()
{

}

void ModMenuModule::CameraPosMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&CameraPosMenu::OnCheatStateChange);
	UpdateCheatStates();
}

void ModMenuModule::CameraPosMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
}

void ModMenuModule::CameraPosMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::CameraPosMenu::AttachCheatMenuItems()
{
	if (m_cheatItemsInnerCont != nullptr) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	CameraPosCheat* cameraPosCheat = GetCheat<CameraPosCheat>();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_cheatItemsInnerCont = uiRoot->AddComponent<UiModule::VertCont>(m_cheatItemsOuterCont);
	UiModule::VertCont* container = m_cheatItemsInnerCont;

	m_menuController->SetCurrentGroupId(1);

	// lock at current position button
	UiModule::Text* lockAtCurrentPosText = m_menuController->CreateItem<UiModule::Text>(container, L"Lock at current position", options.textSize);
	auto lockAtCurrentPosController = m_menuController->CreateLatestItemController<UiModule::ButtonController>(lockAtCurrentPosText);
	lockAtCurrentPosController->SetCallback([cameraPosCheat]() {
		cameraPosCheat->LockAtCurrentPos();
	});

	// snap to target position button
	UiModule::Text* snapToTargetPosText = m_menuController->CreateItem<UiModule::Text>(container, L"Snap to target position", options.textSize);
	auto snapToTargetPosController = m_menuController->CreateLatestItemController<UiModule::ButtonController>(snapToTargetPosText);
	snapToTargetPosController->SetCallback([cameraPosCheat]() {
		cameraPosCheat->SnapToTargetPos();
	});

	// reset position options button
	UiModule::Text* resetPosOptionsText = m_menuController->CreateItem<UiModule::Text>(container, L"Reset position options", options.textSize);
	auto resetPosOptionsController = m_menuController->CreateLatestItemController<UiModule::ButtonController>(resetPosOptionsText);
	resetPosOptionsController->SetCallback([cameraPosCheat]() {
		cameraPosCheat->ResetPosOptions();
	});

	uiRoot->AddComponent<UiModule::Spacer>(container, 0, options.menuSpacerHeight);

	// x position
	UiModule::Text* xPosText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto xPosController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>>(
		xPosText,
		[cameraPosCheat]() {
			return cameraPosCheat->GetOptions().x.value;
		},
		UiModule::VarTextEditableControllerOptions{ L"X: #", L"#" }
	);
	xPosController->SetConverter<ScrfConverter>();
	xPosController->SetCustomSaveCallback([cameraPosCheat](Game::SCR_f newValue) {
		CameraPosCheatOptions options = cameraPosCheat->GetOptions();
		options.x.value = newValue;
		cameraPosCheat->SetOptions(options);
	});

	// y position
	UiModule::Text* yPosText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto yPosController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>>(
		yPosText,
		[cameraPosCheat]() {
			return cameraPosCheat->GetOptions().y.value;
		},
		UiModule::VarTextEditableControllerOptions{ L"Y: #", L"#" }
	);
	yPosController->SetConverter<ScrfConverter>();
	yPosController->SetCustomSaveCallback([cameraPosCheat](Game::SCR_f newValue) {
		CameraPosCheatOptions options = cameraPosCheat->GetOptions();
		options.y.value = newValue;
		options.y.mode = CameraPosCheatMode::LockTargetAt;
		cameraPosCheat->SetOptions(options);
	});

	// z position
	UiModule::Text* zPosText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto zPosController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>>(
		zPosText,
		[cameraPosCheat]() {
			return cameraPosCheat->GetOptions().z.value;
		},
		UiModule::VarTextEditableControllerOptions{ L"Z: #", L"#" }
	);
	zPosController->SetConverter<ScrfConverter>();
	zPosController->SetCustomSaveCallback([cameraPosCheat](Game::SCR_f newValue) {
		CameraPosCheatOptions options = cameraPosCheat->GetOptions();
		options.z.value = newValue;
		options.z.mode = CameraPosCheatMode::LockTargetAt;
		cameraPosCheat->SetOptions(options);
	});

	// zoom
	UiModule::Text* zoomText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto zoomController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>>(
		zoomText,
		[cameraPosCheat]() {
			return cameraPosCheat->GetOptions().zoom.value;
		},
		UiModule::VarTextEditableControllerOptions{ L"Zoom: #", L"#" }
	);
	zoomController->SetConverter<ScrfConverter>();
	zoomController->SetCustomSaveCallback([cameraPosCheat](Game::SCR_f newValue) {
		CameraPosCheatOptions options = cameraPosCheat->GetOptions();
		options.zoom.value = newValue;
		options.zoom.mode = CameraPosCheatMode::LockTargetAt;
		cameraPosCheat->SetOptions(options);
	});

	// reverse z min lock
	UiModule::Text* reverseZMinLockText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto reverseZMinLockController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		reverseZMinLockText,
		[cameraPosCheat]() {
			return cameraPosCheat->GetOptions().reverseZMinLock;
		},
		UiModule::SelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Reverse Z min lock: #", L"#" }
	);
	reverseZMinLockController->SetConverter<EnabledDisabledConverter>();
	reverseZMinLockController->SetCustomSaveCallback([cameraPosCheat](bool newValue) {
		CameraPosCheatOptions options = cameraPosCheat->GetOptions();
		options.reverseZMinLock = newValue;
		cameraPosCheat->SetOptions(options);
	});
}

void ModMenuModule::CameraPosMenu::DetachCheatMenuItems()
{
	m_menuController->DeleteGroupItems(1);
	m_menuController->SetCurrentGroupId(0);

	if (m_cheatItemsInnerCont) {
		UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
		uiRoot->RemoveComponent(m_cheatItemsInnerCont, true);
		m_cheatItemsInnerCont = nullptr;
	}
}

void ModMenuModule::CameraPosMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(ModMenuModule::CameraPosCheat)) {
		m_cameraPosCheatController->SetValue(event.IsEnabled());
		if (event.IsEnabled()) AttachCheatMenuItems();
		else DetachCheatMenuItems();
	}
}

void ModMenuModule::CameraPosMenu::UpdateCheatStates()
{
	bool cheatEnabled = IsCheatEnabled<ModMenuModule::CameraPosCheat>();

	m_cameraPosCheatController->SetValue(cheatEnabled);
	if (cheatEnabled) AttachCheatMenuItems();
	else DetachCheatMenuItems();
}


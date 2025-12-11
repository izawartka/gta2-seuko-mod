#include "camera-pos-menu.h"
#include "../segments/camera-pos-cheat-coord-segment.h"
#include "../root.h"
#include "../cheats/camera/camera-pos.h"
#include "../../../converters/enabled-disabled.h"
#include "../../../converters/camera-pos-cheat-mode.h"
#include "../../../converters/scrf.h"

ModMenuModule::CameraPosMenu::CameraPosMenu()
{
	CreateSegment<CameraPosCheatCoordSegment>(L"X: ", 0);
	CreateSegment<CameraPosCheatCoordSegment>(L"Y: ", 1);
	CreateSegment<CameraPosCheatCoordSegment>(L"Z: ", 2);
	CreateSegment<CameraPosCheatCoordSegment>(L"Zoom: ", 3);
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

	m_cheatItemsCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);
	m_cheatItemsGroupId = m_menuController->RegisterGroup();

	UpdateCheatStates();
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::CameraPosMenu::Detach()
{
	DetachCheatMenuItems();
	DestroyMenu();
}

void ModMenuModule::CameraPosMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&CameraPosMenu::OnCheatStateChange);
	UpdateCheatStates();
	SetSegmentsVisible(true);
}

void ModMenuModule::CameraPosMenu::OnHide()
{
	SetSegmentsVisible(false);
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
	if (m_cheatItemsAttached) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	CameraPosCheat* cameraPosCheat = GetCheat<CameraPosCheat>();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	UiModule::VertCont* container = m_cheatItemsCont;

	m_menuController->SetCurrentGroupId(m_cheatItemsGroupId);

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

	// spacer
	m_cheatItemsSpacer = uiRoot->AddComponent<UiModule::Spacer>(container, 0, options.menuSpacerHeight);

	AttachAllSegments(this, container);

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

	m_cheatItemsAttached = true;
}

void ModMenuModule::CameraPosMenu::DetachCheatMenuItems()
{
	if (!m_cheatItemsAttached) return;

	DetachAllSegments();

	m_menuController->DeleteGroupItems(m_cheatItemsGroupId);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveComponent(m_cheatItemsSpacer, true);
	m_cheatItemsSpacer = nullptr;

	m_cheatItemsAttached = false;
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


#include "smooth-camera-menu.h"
#include "../root.h"
#include "../cheats/camera/smooth-camera.h"
#include "../../../converters/enabled-disabled.h"
#include "smooth-vehicle-camera-menu.h"
#include "smooth-follow-camera-menu.h"

ModMenuModule::SmoothCameraMenu::SmoothCameraMenu()
{

}

ModMenuModule::SmoothCameraMenu::~SmoothCameraMenu()
{

}

bool ModMenuModule::SmoothCameraMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Smooth camera options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	SmoothCameraCheat* smoothCameraCheat = SmoothCameraCheat::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// smooth camera cheat
	UiModule::Text* smoothCameraCheatText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_smoothCameraCheatController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		smoothCameraCheatText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Smooth camera: #", L"#" }
	);
	m_smoothCameraCheatController->SetConverter<EnabledDisabledConverter>();
	m_smoothCameraCheatController->SetSaveCallback([smoothCameraCheat](bool newValue) {
		smoothCameraCheat->SetEnabled(newValue);
	});

	m_cheatItemsCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);
	m_cheatItemsGroupId = m_menuController->RegisterGroup();

	// warning
	uiRoot->AddComponent<UiModule::Spacer>(
		vertCont,
		0,
		options.menuSpacerHeight
	);

	auto* warnMargin = uiRoot->AddComponent<UiModule::Margin>(
		vertCont,
		options.menuControllerOptions.createdSelectableOptions.markerOffsetX,
		0
	);

	auto* warnCont = uiRoot->AddComponent<UiModule::VertCont>(warnMargin);

	uiRoot->AddComponent<UiModule::Text>(
		warnCont,
		L"Warning: This feature is experimental.",
		options.textSize,
		4 // orange color
	);

	uiRoot->AddComponent<UiModule::Text>(
		warnCont,
		L"Saved values may be reset in future releases.",
		options.textSize,
		4 // orange color
	);

	UpdateCheatState();
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SmoothCameraMenu::Detach()
{
	SaveCurrentSelectedIndex();
	DetachCheatMenuItems();
	DestroyMenu();
}

void ModMenuModule::SmoothCameraMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&SmoothCameraMenu::OnCheatStateChange);
	UpdateCheatState();
}

void ModMenuModule::SmoothCameraMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
}

void ModMenuModule::SmoothCameraMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	MenuManager* menuManager = MenuManager::GetInstance();

	if (id == 0) { // Go back
		menuManager->RemoveLastMenu();
		return;
	}
	else if (id == m_carCameraMenuItemId) { // Vehicle camera
		menuManager->AddMenu<SmoothVehicleCameraMenu>();
		return;
	}
	else if (id == m_followCameraMenuItemId) { // Follow camera
		menuManager->AddMenu<SmoothFollowCameraMenu>();
		return;
	}
}

void ModMenuModule::SmoothCameraMenu::AttachCheatMenuItems()
{
	if (m_cheatItemsAttached) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	SmoothCameraCheat* smoothCameraCheat = SmoothCameraCheat::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	UiModule::VertCont* container = m_cheatItemsCont;

	m_menuController->SetCurrentGroupId(m_cheatItemsGroupId);

	// vehicle camera
	m_menuController->CreateItem<UiModule::Text>(container, L"Vehicle camera", options.textSize);
	m_carCameraMenuItemId = m_menuController->GetLatestMenuItemId();

	// follow camera
	m_menuController->CreateItem<UiModule::Text>(container, L"Follow camera", options.textSize);
	m_followCameraMenuItemId = m_menuController->GetLatestMenuItemId();

	// reset options
	auto* resetOptionsText = m_menuController->CreateItem<UiModule::Text>(container, L"Reset options", options.textSize);
	auto* resetOptionsBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(resetOptionsText);
	resetOptionsBtn->SetCallback([smoothCameraCheat]() {
		smoothCameraCheat->SetOptions({});
	});

	m_cheatItemsAttached = true;
}

void ModMenuModule::SmoothCameraMenu::DetachCheatMenuItems()
{
	if (!m_cheatItemsAttached) return;

	m_menuController->DeleteGroupItems(m_cheatItemsGroupId);
	m_cheatItemsAttached = false;

	m_carCameraMenuItemId = -1;
	m_followCameraMenuItemId = -1;
}

void ModMenuModule::SmoothCameraMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(SmoothCameraCheat)) {
		m_smoothCameraCheatController->SetValue(event.IsEnabled());

		if (event.IsEnabled()) AttachCheatMenuItems();
		else DetachCheatMenuItems();
	}
}

void ModMenuModule::SmoothCameraMenu::UpdateCheatState()
{
	bool smoothCameraCheatEnabled = SmoothCameraCheat::GetInstance()->IsEnabled();
	m_smoothCameraCheatController->SetValue(smoothCameraCheatEnabled);

	if (smoothCameraCheatEnabled) AttachCheatMenuItems();
	else DetachCheatMenuItems();
}


#include "smooth-follow-camera-menu.h"
#include "../root.h"
#include "../events/cheat-options-update.h"
#include "../../../converters/enabled-disabled.h"
#include "../../../converters/float.h"
#include "../../../converters/scrf.h"

ModMenuModule::SmoothFollowCameraMenu::SmoothFollowCameraMenu()
{

}

ModMenuModule::SmoothFollowCameraMenu::~SmoothFollowCameraMenu()
{

}

bool ModMenuModule::SmoothFollowCameraMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Smooth follow camera options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	SmoothCameraCheat* smoothCameraCheat = SmoothCameraCheat::GetInstance();
	if (!smoothCameraCheat->IsEnabled()) {
		spdlog::debug("SmoothFollowCameraMenu: SmoothCameraCheat is not enabled, skipping Attach");
		SetPreviousSelectedIndex();

		return true;
	}

	// Smooth follow camera
	UiModule::Text* smoothFollowCameraText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto smoothFollowCameraController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		smoothFollowCameraText,
		[smoothCameraCheat]() {
			return smoothCameraCheat->GetOptions().smoothFollowCamera;
		},
		UiModule::SelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Smooth follow camera: #", L"#" }
	);
	smoothFollowCameraController->SetConverter<EnabledDisabledConverter>();
	smoothFollowCameraController->SetCustomSaveCallback([smoothCameraCheat](bool newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.smoothFollowCamera = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	m_cheatItemsCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);
	m_cheatItemsGroupId = m_menuController->RegisterGroup();

	UpdateCheatState();

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SmoothFollowCameraMenu::Detach()
{
	DetachCheatMenuItems();
	DestroyMenu();
}

void ModMenuModule::SmoothFollowCameraMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&SmoothFollowCameraMenu::OnCheatStateChange);
	AddEventListener<ModMenuModule::CheatOptionsUpdateEvent<SmoothCameraCheat>>(
		&SmoothFollowCameraMenu::OnCheatOptionsUpdate
	);
	UpdateCheatState();
}

void ModMenuModule::SmoothFollowCameraMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
	RemoveEventListener<ModMenuModule::CheatOptionsUpdateEvent<SmoothCameraCheat>>();
}

void ModMenuModule::SmoothFollowCameraMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == 0) { // Go back
		MenuManager::GetInstance()->RemoveLastMenu();
		return;
	}
}

void ModMenuModule::SmoothFollowCameraMenu::AttachCheatMenuItems()
{
	if (m_cheatItemsAttached) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	SmoothCameraCheat* smoothCameraCheat = SmoothCameraCheat::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	UiModule::VertCont* container = m_cheatItemsCont;

	m_menuController->SetCurrentGroupId(m_cheatItemsGroupId);

	// Lerp coefficient
	UiModule::Text* lerpCoefText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto lerpCoefController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		lerpCoefText,
		[smoothCameraCheat]() {
			return smoothCameraCheat->GetOptions().followLerpCoef;
		},
		UiModule::VarTextEditableControllerOptions{ L"Lerp coefficient: #", L"#" }
	);
	lerpCoefController->SetConverter<FloatConverter<float, 3>>();
	lerpCoefController->SetCustomSaveCallback([smoothCameraCheat](float newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.followLerpCoef = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	// Lerp max. delta
	UiModule::Text* lerpMaxDeltaText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto lerpMaxDeltaController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>>(
		lerpMaxDeltaText,
		[smoothCameraCheat]() {
			return smoothCameraCheat->GetOptions().followMaxDelta;
		},
		UiModule::VarTextEditableControllerOptions{ L"Lerp max. delta: #", L"#" }
	);
	lerpMaxDeltaController->SetConverter<ScrfConverter>();
	lerpMaxDeltaController->SetCustomSaveCallback([smoothCameraCheat](Game::SCR_f newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.followMaxDelta = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	// Lerp max. zoom delta
	UiModule::Text* lerpMaxZoomDeltaText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto lerpMaxZoomDeltaController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>>(
		lerpMaxZoomDeltaText,
		[smoothCameraCheat]() {
		return smoothCameraCheat->GetOptions().followZoomMaxDelta;
	},
		UiModule::VarTextEditableControllerOptions{ L"Lerp max. zoom delta: #", L"#" }
	);
	lerpMaxZoomDeltaController->SetConverter<ScrfConverter>();
	lerpMaxZoomDeltaController->SetCustomSaveCallback([smoothCameraCheat](Game::SCR_f newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.followZoomMaxDelta = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	m_cheatItemsAttached = true;
}

void ModMenuModule::SmoothFollowCameraMenu::DetachCheatMenuItems()
{
	if (!m_cheatItemsAttached) return;

	m_menuController->DeleteGroupItems(m_cheatItemsGroupId);
	m_cheatItemsAttached = false;
}

void ModMenuModule::SmoothFollowCameraMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() != typeid(SmoothCameraCheat)) return;
	if (event.IsEnabled()) return;

	MenuManager::GetInstance()->RemoveLastMenu();
}

void ModMenuModule::SmoothFollowCameraMenu::OnCheatOptionsUpdate(CheatOptionsUpdateEvent<SmoothCameraCheat>& event)
{
	bool isSmoothFollowCameraEnabled = event.GetNewOptions().smoothFollowCamera;

	if (isSmoothFollowCameraEnabled) AttachCheatMenuItems();
	else DetachCheatMenuItems();
}

void ModMenuModule::SmoothFollowCameraMenu::UpdateCheatState()
{
	SmoothCameraCheat* smoothCameraCheat = SmoothCameraCheat::GetInstance();

	bool smoothCameraCheatEnabled = smoothCameraCheat->IsEnabled();
	if (!smoothCameraCheatEnabled) {
		MenuManager::GetInstance()->RemoveLastMenu();
		return;
	}

	bool isSmoothVehicleCameraEnabled = smoothCameraCheat->IsEnabled();

	if (isSmoothVehicleCameraEnabled) AttachCheatMenuItems();
	else DetachCheatMenuItems();
}

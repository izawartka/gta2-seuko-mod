#include "smooth-vehicle-camera-menu.h"
#include "../root.h"
#include "../events/cheat-options-update.h"
#include "../../../converters/enabled-disabled.h"
#include "../../../converters/float.h"
#include "../../../converters/scrf.h"
#include "../../../converters/radians.h"

ModMenuModule::SmoothVehicleCameraMenu::SmoothVehicleCameraMenu()
{

}

ModMenuModule::SmoothVehicleCameraMenu::~SmoothVehicleCameraMenu()
{

}

bool ModMenuModule::SmoothVehicleCameraMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Smooth vehicle camera options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	SmoothCameraCheat* smoothCameraCheat = SmoothCameraCheat::GetInstance();
	if (!smoothCameraCheat->IsEnabled()) {
		spdlog::debug("SmoothVehicleCameraMenu: SmoothCameraCheat is not enabled, skipping Attach");
		SetPreviousSelectedIndex();

		return true;
	}

	// Smooth car camera
	UiModule::Text* smoothVehicleCameraText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto smoothVehicleCameraController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		smoothVehicleCameraText,
		[smoothCameraCheat]() {
		return smoothCameraCheat->GetOptions().smoothCarCamera;
	},
		UiModule::SelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Smooth vehicle camera: #", L"#" }
	);
	smoothVehicleCameraController->SetConverter<EnabledDisabledConverter>();
	smoothVehicleCameraController->SetCustomSaveCallback([smoothCameraCheat](bool newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.smoothCarCamera = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	m_cheatItemsCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);
	m_cheatItemsGroupId = m_menuController->RegisterGroup();

	UpdateCheatState();
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SmoothVehicleCameraMenu::Detach()
{
	DetachCheatMenuItems();
	DestroyMenu();
}

void ModMenuModule::SmoothVehicleCameraMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&SmoothVehicleCameraMenu::OnCheatStateChange);
	AddEventListener<ModMenuModule::CheatOptionsUpdateEvent<SmoothCameraCheat>>(
		&SmoothVehicleCameraMenu::OnCheatOptionsUpdate
	);
	UpdateCheatState();
}

void ModMenuModule::SmoothVehicleCameraMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
	RemoveEventListener<ModMenuModule::CheatOptionsUpdateEvent<SmoothCameraCheat>>();
}

void ModMenuModule::SmoothVehicleCameraMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == 0) { // Go back
		MenuManager::GetInstance()->RemoveLastMenu();
		return;
	}
}

void ModMenuModule::SmoothVehicleCameraMenu::AttachCheatMenuItems()
{
	if (m_cheatItemsAttached) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	SmoothCameraCheat* smoothCameraCheat = SmoothCameraCheat::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	UiModule::VertCont* container = m_cheatItemsCont;

	m_menuController->SetCurrentGroupId(m_cheatItemsGroupId);

	// Horizontal move scale
	UiModule::Text* horMoveScaleText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto horMoveScaleController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		horMoveScaleText,
		[smoothCameraCheat]() {
		return smoothCameraCheat->GetOptions().carCameraHorMoveScale;
	},
		UiModule::VarTextEditableControllerOptions{ L"Horizontal move scale: #", L"#" }
	);
	horMoveScaleController->SetConverter<FloatConverter<float, 3>>();
	horMoveScaleController->SetCustomSaveCallback([smoothCameraCheat](float newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.carCameraHorMoveScale = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	// Vertical move scale
	UiModule::Text* vertMoveScaleText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto vertMoveScaleController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		vertMoveScaleText,
		[smoothCameraCheat]() {
		return smoothCameraCheat->GetOptions().carCameraVertMoveScale;
	},
		UiModule::VarTextEditableControllerOptions{ L"Vertical move scale: #", L"#" }
	);
	vertMoveScaleController->SetConverter<FloatConverter<float, 3>>();
	vertMoveScaleController->SetCustomSaveCallback([smoothCameraCheat](float newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.carCameraVertMoveScale = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	// Magnitude lerp coefficient
	UiModule::Text* magLerpCoefText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto magLerpCoefController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		magLerpCoefText,
		[smoothCameraCheat]() {
			return smoothCameraCheat->GetOptions().carCameraMagLerpCoef;
		},
		UiModule::VarTextEditableControllerOptions{ L"Magnitude lerp coefficient: #", L"#" }
	);
	magLerpCoefController->SetConverter<FloatConverter<float, 3>>();
	magLerpCoefController->SetCustomSaveCallback([smoothCameraCheat](float newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.carCameraMagLerpCoef = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	// Magnitude lerp max. delta
	UiModule::Text* magLerpMaxDeltaText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto magLerpMaxDeltaController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>>(
		magLerpMaxDeltaText,
		[smoothCameraCheat]() {
			return smoothCameraCheat->GetOptions().carCameraMagMaxDelta;
		},
		UiModule::VarTextEditableControllerOptions{ L"Magnitude lerp max. delta: #", L"#" }
	);
	magLerpMaxDeltaController->SetConverter<ScrfConverter>();
	magLerpMaxDeltaController->SetCustomSaveCallback([smoothCameraCheat](Game::SCR_f newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.carCameraMagMaxDelta = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	// Hor. rot. lerp coefficient
	UiModule::Text* horLerpCoefText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto horLerpCoefController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		horLerpCoefText,
		[smoothCameraCheat]() {
			return smoothCameraCheat->GetOptions().carCameraHorLerpCoef;
		},
		UiModule::VarTextEditableControllerOptions{ L"Hor. rot. lerp coefficient: #", L"#" }
	);
	horLerpCoefController->SetConverter<FloatConverter<float, 3>>();
	horLerpCoefController->SetCustomSaveCallback([smoothCameraCheat](float newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.carCameraHorLerpCoef = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	// Hor. rot. lerp max. delta
	UiModule::Text* horLerpMaxDeltaText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto horLerpMaxDeltaController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		horLerpMaxDeltaText,
		[smoothCameraCheat]() {
			return smoothCameraCheat->GetOptions().carCameraHorMaxDelta;
		},
		UiModule::VarTextEditableControllerOptions{ L"Hor. rot. lerp max. delta: #", L" deg#" }
	);
	horLerpMaxDeltaController->SetConverter<RadiansConverter<>>();
	horLerpMaxDeltaController->SetCustomSaveCallback([smoothCameraCheat](float newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.carCameraHorMaxDelta = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	// Vert. rot. lerp coefficient
	UiModule::Text* vertLerpCoefText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto vertLerpCoefController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		vertLerpCoefText,
		[smoothCameraCheat]() {
		return smoothCameraCheat->GetOptions().carCameraVertLerpCoef;
	},
		UiModule::VarTextEditableControllerOptions{ L"Vert. rot. lerp coefficient: #", L"#" }
	);
	vertLerpCoefController->SetConverter<FloatConverter<float, 3>>();
	vertLerpCoefController->SetCustomSaveCallback([smoothCameraCheat](float newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.carCameraVertLerpCoef = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	// Vert. rot. lerp max. delta
	UiModule::Text* vertLerpMaxDeltaText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
	auto vertLerpMaxDeltaController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		vertLerpMaxDeltaText,
		[smoothCameraCheat]() {
		return smoothCameraCheat->GetOptions().carCameraVertMaxDelta;
	},
		UiModule::VarTextEditableControllerOptions{ L"Vert. rot. lerp max. delta: #", L" deg#" }
	);
	vertLerpMaxDeltaController->SetConverter<RadiansConverter<>>();
	vertLerpMaxDeltaController->SetCustomSaveCallback([smoothCameraCheat](float newValue) {
		SmoothCameraCheatOptions options = smoothCameraCheat->GetOptions();
		options.carCameraVertMaxDelta = newValue;
		smoothCameraCheat->SetOptions(options);
	});

	m_cheatItemsAttached = true;
}

void ModMenuModule::SmoothVehicleCameraMenu::DetachCheatMenuItems()
{
	if (!m_cheatItemsAttached) return;

	m_menuController->DeleteGroupItems(m_cheatItemsGroupId);
	m_cheatItemsAttached = false;
}

void ModMenuModule::SmoothVehicleCameraMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() != typeid(SmoothCameraCheat)) return;
	if (event.IsEnabled()) return;

	MenuManager::GetInstance()->RemoveLastMenu();
}

void ModMenuModule::SmoothVehicleCameraMenu::OnCheatOptionsUpdate(CheatOptionsUpdateEvent<SmoothCameraCheat>& event)
{
	bool isSmoothVehicleCameraEnabled = event.GetNewOptions().smoothCarCamera;

	if (isSmoothVehicleCameraEnabled) AttachCheatMenuItems();
	else DetachCheatMenuItems();
}

void ModMenuModule::SmoothVehicleCameraMenu::UpdateCheatState()
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

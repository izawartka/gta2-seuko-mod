#include "camera-menu.h"
#include "camera-pos-menu.h"
#include "../root.h"
#include "../cheats/camera/camera.h"
#include "../cheats/camera/clear-screen.h"
#include "../cheats/camera/disable-culling.h"
#include "../../../converters/enabled-disabled.h"
#include "../../../converters/radians.h"
#include "../../../converters/yes-no.h"

ModMenuModule::CameraMenu::CameraMenu()
{

}

ModMenuModule::CameraMenu::~CameraMenu()
{

}

bool ModMenuModule::CameraMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Camera#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	CameraCheat* cameraCheat = GetCheat<CameraCheat>();
	ClearScreenCheat* clearScreenCheat = GetCheat<ClearScreenCheat>();
	DisableCullingCheat* disableCullingCheat = GetCheat<DisableCullingCheat>();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Camera position", options.textSize);

	// camera cheat
	UiModule::Text* cameraCheatText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_cameraCheatController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		cameraCheatText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Camera cheat: #", L"#" }
	);
	m_cameraCheatController->SetConverter<EnabledDisabledConverter>();
	m_cameraCheatController->SetSaveCallback([cameraCheat](bool newValue) {
		cameraCheat->SetEnabled(newValue);
	});

	// vertical angle
	UiModule::Text* verticalAngleText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto vertAngleController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		verticalAngleText,
		[cameraCheat]() {
			return cameraCheat->GetOptions().cameraTransform.verticalAngleRad;
		},
		UiModule::VarTextEditableControllerOptions{ L"Vertical angle: #", L" deg #" }
	);
	vertAngleController->SetConverter<RadiansConverter<>>();
	vertAngleController->SetCustomSaveCallback([cameraCheat](float newValue) {
		CameraCheatOptions options = cameraCheat->GetOptions();
		options.cameraTransform.verticalAngleRad = newValue;
		cameraCheat->SetOptions(options);
	});

	// horizontal angle
	UiModule::Text* horizontalAngleText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto horizAngleController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		horizontalAngleText,
		[cameraCheat]() {
			return cameraCheat->GetOptions().cameraTransform.horizontalAngleRad;
		},
		UiModule::VarTextEditableControllerOptions{ L"Horizontal angle: #", L" deg #" }
	);
	horizAngleController->SetConverter<RadiansConverter<>>();
	horizAngleController->SetCustomSaveCallback([cameraCheat](float newValue) {
		CameraCheatOptions options = cameraCheat->GetOptions();
		options.cameraTransform.horizontalAngleRad = newValue;
		cameraCheat->SetOptions(options);
	});

	// additional z offset
	UiModule::Text* additionalZOffsetText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto addZOffsetController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<float, float>>(
		additionalZOffsetText,
		[cameraCheat]() {
			return cameraCheat->GetOptions().cameraTransform.additionalZOffset;
		},
		UiModule::VarTextEditableControllerOptions{ L"Additional Z offset: #", L"#" }
	);
	addZOffsetController->SetCustomSaveCallback([cameraCheat](float newValue) {
		CameraCheatOptions options = cameraCheat->GetOptions();
		options.cameraTransform.additionalZOffset = newValue;
		cameraCheat->SetOptions(options);
	});

	// custom culling
	UiModule::Text* customCullingText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto customCullingController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		customCullingText,
		[cameraCheat]() {
		return cameraCheat->GetOptions().customCulling;
	},
		UiModule::SelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Custom culling: #", L"#" }
	);
	customCullingController->SetConverter<YesNoConverter>();
	customCullingController->SetCustomSaveCallback([cameraCheat](bool newValue) {
		CameraCheatOptions options = cameraCheat->GetOptions();
		options.customCulling = newValue;
		cameraCheat->SetOptions(options);
	});

	// follow ped rotation
	UiModule::Text* followPedRotationText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto followPedRotationController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		followPedRotationText,
		[cameraCheat]() {
		return cameraCheat->GetOptions().followPedRotation;
	},
		UiModule::SelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Follow ped rotation: #", L"#" }
	);
	followPedRotationController->SetConverter<YesNoConverter>();
	followPedRotationController->SetCustomSaveCallback([cameraCheat](bool newValue) {
		CameraCheatOptions options = cameraCheat->GetOptions();
		options.followPedRotation = newValue;
		cameraCheat->SetOptions(options);
	});

	// custom render queue
	UiModule::Text* customRenderQueueText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto customRenderQueueController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		customRenderQueueText,
		[cameraCheat]() {
		return cameraCheat->GetOptions().customRenderQueue;
	},
		UiModule::SelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Custom render queue: #", L"#" }
	);
	customRenderQueueController->SetConverter<YesNoConverter>();
	customRenderQueueController->SetCustomSaveCallback([cameraCheat](bool newValue) {
		CameraCheatOptions options = cameraCheat->GetOptions();
		options.customRenderQueue = newValue;
		cameraCheat->SetOptions(options);
	});

	// render distance
	UiModule::Text* renderDistanceText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto renderDistanceController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<size_t, size_t>>(
		renderDistanceText,
		[cameraCheat]() {
		return cameraCheat->GetOptions().renderDistance;
	},
		UiModule::VarTextEditableControllerOptions{ L"Render distance: #", L"#" }
	);
	renderDistanceController->SetCustomSaveCallback([cameraCheat](size_t newValue) {
		CameraCheatOptions options = cameraCheat->GetOptions();
		options.renderDistance = newValue;
		cameraCheat->SetOptions(options);
	});

	// clear screen
	UiModule::Text* clearScreenText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_clearScreenCheatController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		clearScreenText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Force clear screen: #", L"#" }
	);
	m_clearScreenCheatController->SetConverter<EnabledDisabledConverter>();
	m_clearScreenCheatController->SetSaveCallback([clearScreenCheat](bool newValue) {
		clearScreenCheat->SetEnabled(newValue);
	});

	// disable default culling
	UiModule::Text* disableCullingText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_disableCullingCheatController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		disableCullingText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Disable default culling: #", L"#" }
	);
	m_disableCullingCheatController->SetConverter<EnabledDisabledConverter>();
	m_disableCullingCheatController->SetSaveCallback([disableCullingCheat](bool newValue) {
		disableCullingCheat->SetEnabled(newValue);
	});
	
	UpdateCheatStates();
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::CameraMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&CameraMenu::OnCheatStateChange);
	UpdateCheatStates();
}

void ModMenuModule::CameraMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
}

void ModMenuModule::CameraMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Camera position
		ModMenuModule::MenuManager::GetInstance()->AddMenu<CameraPosMenu>();
		break;
	default:
		break;
	}
}

void ModMenuModule::CameraMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(ModMenuModule::CameraCheat)) {
		m_cameraCheatController->SetValue(event.IsEnabled());
	}
	else if (event.GetCheatType() == typeid(ModMenuModule::ClearScreenCheat)) {
		m_clearScreenCheatController->SetValue(event.IsEnabled());
	}
	else if (event.GetCheatType() == typeid(ModMenuModule::DisableCullingCheat)) {
		m_disableCullingCheatController->SetValue(event.IsEnabled());
	}
}

void ModMenuModule::CameraMenu::UpdateCheatStates()
{
	m_cameraCheatController->SetValue(IsCheatEnabled<ModMenuModule::CameraCheat>());
	m_clearScreenCheatController->SetValue(IsCheatEnabled<ModMenuModule::ClearScreenCheat>());
	m_disableCullingCheatController->SetValue(IsCheatEnabled<ModMenuModule::DisableCullingCheat>());
}


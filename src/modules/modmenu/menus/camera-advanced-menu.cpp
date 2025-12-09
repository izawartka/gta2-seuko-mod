#include "camera-advanced-menu.h"
#include "../root.h"
#include "../cheats/camera/camera.h"
#include "../cheats/camera/clear-screen.h"
#include "../cheats/camera/disable-culling.h"
#include "../cheats/camera/shadows-fix.h"
#include "../../../converters/enabled-disabled.h"
#include "../../../converters/radians.h"
#include "../../../converters/yes-no.h"

ModMenuModule::CameraAdvancedMenu::CameraAdvancedMenu()
{

}

ModMenuModule::CameraAdvancedMenu::~CameraAdvancedMenu()
{

}

bool ModMenuModule::CameraAdvancedMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Advanced camera options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	CameraCheat* cameraCheat = GetCheat<CameraCheat>();
	ClearScreenCheat* clearScreenCheat = GetCheat<ClearScreenCheat>();
	DisableCullingCheat* disableCullingCheat = GetCheat<DisableCullingCheat>();
	ShadowsFixCheat* shadowsFixCheat = GetCheat<ShadowsFixCheat>();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

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

	// shadows fix
	UiModule::Text* shadowsFixText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_shadowsFixCheatController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		shadowsFixText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Shadows fix: #", L"#" }
	);
	m_shadowsFixCheatController->SetConverter<EnabledDisabledConverter>();
	m_shadowsFixCheatController->SetSaveCallback([shadowsFixCheat](bool newValue) {
		shadowsFixCheat->SetEnabled(newValue);
	});

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

	m_cheatItemsCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);
	m_cheatItemsGroupId = m_menuController->RegisterGroup();

	UpdateCheatStates();
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::CameraAdvancedMenu::Detach()
{
	DetachCheatMenuItems();
	DestroyMenu();
}

void ModMenuModule::CameraAdvancedMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&CameraAdvancedMenu::OnCheatStateChange);
	UpdateCheatStates();
}

void ModMenuModule::CameraAdvancedMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
}

void ModMenuModule::CameraAdvancedMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::CameraAdvancedMenu::AttachCheatMenuItems()
{
	if (m_cheatItemsAttached) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	CameraCheat* cameraCheat = GetCheat<CameraCheat>();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	UiModule::VertCont* container = m_cheatItemsCont;

	m_menuController->SetCurrentGroupId(m_cheatItemsGroupId);

	// vertical angle
	UiModule::Text* verticalAngleText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
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
	UiModule::Text* horizontalAngleText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
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
	UiModule::Text* additionalZOffsetText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
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
	UiModule::Text* customCullingText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
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
	UiModule::Text* followPedRotationText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
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
	UiModule::Text* customRenderQueueText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
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
	UiModule::Text* renderDistanceText = m_menuController->CreateItem<UiModule::Text>(container, L"", options.textSize);
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

	m_cheatItemsAttached = true;
}

void ModMenuModule::CameraAdvancedMenu::DetachCheatMenuItems()
{
	if (!m_cheatItemsAttached) return;

	m_menuController->DeleteGroupItems(m_cheatItemsGroupId);
	m_cheatItemsAttached = false;
}

void ModMenuModule::CameraAdvancedMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(ModMenuModule::ClearScreenCheat)) {
		m_clearScreenCheatController->SetValue(event.IsEnabled());
	}
	else if (event.GetCheatType() == typeid(ModMenuModule::DisableCullingCheat)) {
		m_disableCullingCheatController->SetValue(event.IsEnabled());
	}
	else if (event.GetCheatType() == typeid(ModMenuModule::ShadowsFixCheat)) {
		m_shadowsFixCheatController->SetValue(event.IsEnabled());
	}
	else if (event.GetCheatType() == typeid(ModMenuModule::CameraCheat)) {
		m_cameraCheatController->SetValue(event.IsEnabled());

		if(event.IsEnabled()) AttachCheatMenuItems();
		else DetachCheatMenuItems();
	}
}

void ModMenuModule::CameraAdvancedMenu::UpdateCheatStates()
{
	m_clearScreenCheatController->SetValue(IsCheatEnabled<ModMenuModule::ClearScreenCheat>());
	m_disableCullingCheatController->SetValue(IsCheatEnabled<ModMenuModule::DisableCullingCheat>());
	m_shadowsFixCheatController->SetValue(IsCheatEnabled<ModMenuModule::ShadowsFixCheat>());

	bool cameraCheatEnabled = IsCheatEnabled<ModMenuModule::CameraCheat>();
	m_cameraCheatController->SetValue(cameraCheatEnabled);

	if (cameraCheatEnabled) AttachCheatMenuItems();
	else DetachCheatMenuItems();
}


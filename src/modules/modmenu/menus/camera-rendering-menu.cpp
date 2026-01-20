#include "camera-rendering-menu.h"
#include "../root.h"
#include "../cheats/camera/clear-screen.h"
#include "../cheats/camera/disable-culling.h"
#include "../cheats/camera/shadows-fix.h"
#include "../../../converters/enabled-disabled.h"

ModMenuModule::CameraRenderingMenu::CameraRenderingMenu()
{

}

ModMenuModule::CameraRenderingMenu::~CameraRenderingMenu()
{

}

bool ModMenuModule::CameraRenderingMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Advanced rendering options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	ClearScreenCheat* clearScreenCheat = ClearScreenCheat::GetInstance();
	DisableCullingCheat* disableCullingCheat = DisableCullingCheat::GetInstance();
	ShadowsFixCheat* shadowsFixCheat = ShadowsFixCheat::GetInstance();

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

	return true;
}

void ModMenuModule::CameraRenderingMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&CameraRenderingMenu::OnCheatStateChange);
	UpdateCheatStates();
}

void ModMenuModule::CameraRenderingMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
}

void ModMenuModule::CameraRenderingMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::CameraRenderingMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(ClearScreenCheat)) {
		m_clearScreenCheatController->SetValue(event.IsEnabled());
	}
	else if (event.GetCheatType() == typeid(DisableCullingCheat)) {
		m_disableCullingCheatController->SetValue(event.IsEnabled());
	}
	else if (event.GetCheatType() == typeid(ShadowsFixCheat)) {
		m_shadowsFixCheatController->SetValue(event.IsEnabled());
	}
}

void ModMenuModule::CameraRenderingMenu::UpdateCheatStates()
{
	m_clearScreenCheatController->SetValue(ClearScreenCheat::GetInstance()->IsEnabled());
	m_disableCullingCheatController->SetValue(DisableCullingCheat::GetInstance()->IsEnabled());
	m_shadowsFixCheatController->SetValue(ShadowsFixCheat::GetInstance()->IsEnabled());
}


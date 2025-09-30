#include "player-pos-menu.h"
#include "../root.h"

ModMenuModule::PlayerPosMenu::PlayerPosMenu()
{

}

ModMenuModule::PlayerPosMenu::~PlayerPosMenu()
{

}

bool ModMenuModule::PlayerPosMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Player position#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	Core::Resolver<Game::Sprite> spriteResolver = Core::MakeResolver(
		[]() { return Game::Memory::GetGame(); },
		mem(&Game::Game::players), idx(0),
		mem(&Game::Player::ped),
		mem(&Game::Ped::gameObject),
		mem(&Game::GameObject::sprite)
	);

	auto onEditStop = [this]() {
		if(m_visible) m_menuController->SetActive(true);
	};

	UiModule::Text* xText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_xController = uiRoot->AddController<UiModule::VarTextEditableController<Game::SCR_f>>(
		xText,
		Core::MakeResolver(spriteResolver, mem(&Game::Sprite::x)),
		UiModule::VarTextEditableControllerOptions{ L"X: #", L"#" }
	);
	m_xController->SetOnEditStopCallback(onEditStop);

	UiModule::Text* yText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_yController = uiRoot->AddController<UiModule::VarTextEditableController<Game::SCR_f>>(
		yText,
		Core::MakeResolver(spriteResolver, mem(&Game::Sprite::y)),
		UiModule::VarTextEditableControllerOptions{ L"Y: #", L"#" }
	);
	m_yController->SetOnEditStopCallback(onEditStop);

	UiModule::Text* zText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_zController = uiRoot->AddController<UiModule::VarTextEditableController<Game::SCR_f>>(
		zText,
		Core::MakeResolver(spriteResolver, mem(&Game::Sprite::z)),
		UiModule::VarTextEditableControllerOptions{ L"Z: #", L"#" }
	);
	m_zController->SetOnEditStopCallback(onEditStop);

	return true;
}

void ModMenuModule::PlayerPosMenu::Detach()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController(m_xController);
	uiRoot->RemoveController(m_yController);
	uiRoot->RemoveController(m_zController);
	DestroyMenu();
}

void ModMenuModule::PlayerPosMenu::OnShow()
{
	m_xController->SetWatching(true);
	m_yController->SetWatching(true);
	m_zController->SetWatching(true);
}

void ModMenuModule::PlayerPosMenu::OnHide()
{
	m_xController->SetWatching(false);
	m_xController->SetEditing(false);
	m_yController->SetWatching(false);
	m_yController->SetEditing(false);
	m_zController->SetWatching(false);
	m_zController->SetEditing(false);
}

void ModMenuModule::PlayerPosMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	case 1: // X
		m_menuController->SetActive(false);
		m_xController->SetEditing(true);
		break;
	case 2: // Y
		m_menuController->SetActive(false);
		m_yController->SetEditing(true);
		break;
	case 3: // Z
		m_menuController->SetActive(false);
		m_zController->SetEditing(true);
		break;
	default:
		break;
	}
}

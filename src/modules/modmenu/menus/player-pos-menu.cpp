#include "player-pos-menu.h"
#include "../root.h"
#include "../../../converters/scrf.h"

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
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::gameObject),
		mem(&Game::GameObject::sprite)
	);

	// x position
	UiModule::Text* xText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto xController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f>>(
		xText,
		Core::MakeResolver(spriteResolver, mem(&Game::Sprite::x)),
		UiModule::VarTextEditableControllerOptions{ L"X: #", L"#" }
	);
	auto onXSave = [this, spriteResolver](Game::SCR_f newX) {
		if (newX <= 0) return;
		if (newX >= Game::Utils::FromFloat(256.0f)) return;
		Game::Sprite* sprite = spriteResolver();
		if (sprite) {
			Game::Functions::SetSpritePosition(sprite, 0, newX, sprite->y, sprite->z);
		}
	};
	xController->SetCustomSaveCallback(onXSave);
	xController->SetConverter<ScrfConverter>();

	// y position
	UiModule::Text* yText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto yController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f>>(
		yText,
		Core::MakeResolver(spriteResolver, mem(&Game::Sprite::y)),
		UiModule::VarTextEditableControllerOptions{ L"Y: #", L"#" }
	);
	auto onYSave = [this, spriteResolver](Game::SCR_f newY) {
		if (newY <= 0) return;
		if (newY >= Game::Utils::FromFloat(256.0f)) return;
		Game::Sprite* sprite = spriteResolver();
		if (sprite) {
			Game::Functions::SetSpritePosition(sprite, 0, sprite->x, newY, sprite->z);
		}
	};
	yController->SetCustomSaveCallback(onYSave);
	yController->SetConverter<ScrfConverter>();

	// z position
	UiModule::Text* zText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto zController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f>>(
		zText,
		Core::MakeResolver(spriteResolver, mem(&Game::Sprite::z)),
		UiModule::VarTextEditableControllerOptions{ L"Z: #", L"#" }
	);
	auto onZSave = [this, spriteResolver](Game::SCR_f newZ) {
		if (newZ <= 0) return;
		if (newZ >= Game::Utils::FromFloat(8.0f)) return;
		Game::Sprite* sprite = spriteResolver();
		if (sprite) {
			Game::Functions::SetSpritePosition(sprite, 0, sprite->x, sprite->y, newZ);
		}
	};
	zController->SetCustomSaveCallback(onZSave);
	zController->SetConverter<ScrfConverter>();

	ApplyIndexPersistence("ModMenu_PlayerPosMenu_SelectedIndex");

	return true;
}

void ModMenuModule::PlayerPosMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

#include "last-vehicle-physics-menu.h"
#include "../root.h"
#include "../cheats/last-car.h"
#include "../../../converters/bit-yes-no.h"

ModMenuModule::LastVehiclePhysicsMenu::LastVehiclePhysicsMenu()
{

}

ModMenuModule::LastVehiclePhysicsMenu::~LastVehiclePhysicsMenu()
{

}

bool ModMenuModule::LastVehiclePhysicsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Vehicle physics#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	auto lastCarResolver = [this]() {
		return this->GetLastCar();
	};

	auto lastCarPhysicsResolver = Core::MakeResolver(
		lastCarResolver,
		mem(&Game::Car::physicsBitmask)
	);

	auto lastCarPhysicsOnSaveGenerator = [lastCarPhysicsResolver, this](short mask) {
		return [lastCarPhysicsResolver, mask, this](short newValue) {
			short* physicsBitmask = lastCarPhysicsResolver();
			if (physicsBitmask) {
				*physicsBitmask = *physicsBitmask & ~mask | newValue;
			}
		};
	};

	// go back
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// no collisions
	UiModule::Text* noCollisionsText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto noCollisionsController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<short>>(
		noCollisionsText,
		lastCarPhysicsResolver,
		UiModule::VarTextSelectOptionList<short>{ 0, Game::CAR_PHYSICS_BITMASK_NO_COLLISION },
		UiModule::VarTextSelectControllerOptions{ L"No collisions: #", L"#" }
	);
	noCollisionsController->SetConverter<BitYesNoConverter<short, Game::CAR_PHYSICS_BITMASK_NO_COLLISION>>();
	noCollisionsController->SetCustomSaveCallback(lastCarPhysicsOnSaveGenerator(Game::CAR_PHYSICS_BITMASK_NO_COLLISION));

	// all invulnerability
	constexpr short allInvulnMask =
		Game::CAR_PHYSICS_BITMASK_NO_COLLISION_DAMAGE |
		Game::CAR_PHYSICS_BITMASK_BULLETPROOF |
		Game::CAR_PHYSICS_BITMASK_ROCKETPROOF |
		Game::CAR_PHYSICS_BITMASK_FLAMEPROOF;

	UiModule::Text* allInvulnText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto allInvulnController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<short>>(
		allInvulnText,
		lastCarPhysicsResolver,
		UiModule::VarTextSelectOptionList<short>{ 0, allInvulnMask },
		UiModule::VarTextSelectControllerOptions{ L"All invulnerability: #", L"#" }
	);
	allInvulnController->SetConverter<BitYesNoConverter<short, allInvulnMask, true>>();
	allInvulnController->SetCustomSaveCallback(lastCarPhysicsOnSaveGenerator(allInvulnMask));

	// collisions invulnerability
	UiModule::Text* collisionsInvulnText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto collisionsInvulnController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<short>>(
		collisionsInvulnText,
		lastCarPhysicsResolver,
		UiModule::VarTextSelectOptionList<short>{ 0, Game::CAR_PHYSICS_BITMASK_NO_COLLISION_DAMAGE },
		UiModule::VarTextSelectControllerOptions{ L"Collisions invulnerability: #", L"#" }
	);
	collisionsInvulnController->SetConverter<BitYesNoConverter<short, Game::CAR_PHYSICS_BITMASK_NO_COLLISION_DAMAGE>>();
	collisionsInvulnController->SetCustomSaveCallback(lastCarPhysicsOnSaveGenerator(Game::CAR_PHYSICS_BITMASK_NO_COLLISION_DAMAGE));

	// bullet invulnerability
	UiModule::Text* bulletInvulnText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto bulletInvulnController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<short>>(
		bulletInvulnText,
		lastCarPhysicsResolver,
		UiModule::VarTextSelectOptionList<short>{ 0, Game::CAR_PHYSICS_BITMASK_BULLETPROOF },
		UiModule::VarTextSelectControllerOptions{ L"Bullet invulnerability: #", L"#" }
	);
	bulletInvulnController->SetConverter<BitYesNoConverter<short, Game::CAR_PHYSICS_BITMASK_BULLETPROOF>>();
	bulletInvulnController->SetCustomSaveCallback(lastCarPhysicsOnSaveGenerator(Game::CAR_PHYSICS_BITMASK_BULLETPROOF));

	// rocket invulnerability
	UiModule::Text* rocketInvulnText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto rocketInvulnController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<short>>(
		rocketInvulnText,
		lastCarPhysicsResolver,
		UiModule::VarTextSelectOptionList<short>{ 0, Game::CAR_PHYSICS_BITMASK_ROCKETPROOF },
		UiModule::VarTextSelectControllerOptions{ L"Rocket invulnerability: #", L"#" }
	);
	rocketInvulnController->SetConverter<BitYesNoConverter<short, Game::CAR_PHYSICS_BITMASK_ROCKETPROOF>>();
	rocketInvulnController->SetCustomSaveCallback(lastCarPhysicsOnSaveGenerator(Game::CAR_PHYSICS_BITMASK_ROCKETPROOF));

	// fire invulnerability
	UiModule::Text* fireInvulnText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto fireInvulnController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<short>>(
		fireInvulnText,
		lastCarPhysicsResolver,
		UiModule::VarTextSelectOptionList<short>{ 0, Game::CAR_PHYSICS_BITMASK_FLAMEPROOF },
		UiModule::VarTextSelectControllerOptions{ L"Fire invulnerability: #", L"#" }
	);
	fireInvulnController->SetConverter<BitYesNoConverter<short, Game::CAR_PHYSICS_BITMASK_FLAMEPROOF>>();
	fireInvulnController->SetCustomSaveCallback(lastCarPhysicsOnSaveGenerator(Game::CAR_PHYSICS_BITMASK_FLAMEPROOF));

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::LastVehiclePhysicsMenu::OnShow()
{
	AddEventListener<LastCarStateEvent>(&ModMenuModule::LastVehiclePhysicsMenu::OnLastCarStateChange);
	UpdateLastCarState();
}

void ModMenuModule::LastVehiclePhysicsMenu::OnHide()
{
	RemoveEventListener<LastCarStateEvent>();
}

void ModMenuModule::LastVehiclePhysicsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::LastVehiclePhysicsMenu::OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event)
{
	UpdateLastCarState();
}

void ModMenuModule::LastVehiclePhysicsMenu::UpdateLastCarState()
{
	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	if (!lastCarCheat->IsEnabled() || lastCarCheat->GetLastCarState() == LastCarState::NoCar) {
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
	}
}

Game::Car* ModMenuModule::LastVehiclePhysicsMenu::GetLastCar()
{
	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	if (!lastCarCheat->IsEnabled()) {
		return nullptr;
	}
	return lastCarCheat->GetLastCar();
}

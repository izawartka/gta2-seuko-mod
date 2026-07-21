#include "player-pos-menu.h"
#include "../root.h"
#include "../segments/position-rotation-segment.h"
#include "../cheats/player-pos.h"

ModMenuModule::PlayerPosMenu::PlayerPosMenu()
{
	m_posRotSegment = CreateSegment<PositionRotationSegment>("ModMenu_PlayerPosMenu_PosRotSegment");
}

ModMenuModule::PlayerPosMenu::~PlayerPosMenu()
{

}

bool ModMenuModule::PlayerPosMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Player position#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// position rotation segment
	AttachSegment(m_posRotSegment, this, vertCont);

	// teleport btn
	UiModule::Text* teleportText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Teleport", options.textSize);
	UiModule::ButtonController* teleportBtnController = m_menuController->CreateLatestItemController<UiModule::ButtonController>(
		teleportText,
		UiModule::ButtonControllerOptions{}
	);
	teleportBtnController->SetCallback([this]() {
		Teleport();
	});

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PlayerPosMenu::Detach()
{
	DetachSegment(m_posRotSegment);
	DestroyMenu();
}

void ModMenuModule::PlayerPosMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::PlayerPosMenu::OnHide()
{
	SetSegmentsVisible(false);
}

void ModMenuModule::PlayerPosMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::PlayerPosMenu::Teleport()
{
	auto segmentDataOpt = m_posRotSegment->GetSegmentData();
	if (!segmentDataOpt.has_value()) return;

	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();
	if (!playerPosCheat->IsEnabled()) {
		spdlog::error("PlayerPosMenu::Teleport: PlayerPosCheat is not enabled");
		return;
	}

	short targetRotation = segmentDataOpt->rotation;
	playerPosCheat->Teleport(segmentDataOpt->position, [this, targetRotation](bool success) {
		m_posRotSegment->SetDoUpdatePosition(true);

		if(!success) {
			ToastManager::GetInstance()->Show({ L"Teleport failed", ToastType::Error });
			return;
		}

		short* rotationPtr = Game::Utils::GetPlayerPedRotationPtr();
		if(rotationPtr)	*rotationPtr = targetRotation;
	});
}

#include "player-pos-menu.h"
#include "../root.h"
#include "../segments/position-segment.h"
#include "../cheats/player-pos.h"

ModMenuModule::PlayerPosMenu::PlayerPosMenu()
{
	m_positionSegment = CreateSegment<PositionSegment>("Segment_PlayerPosMenu_PositionSegment");
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

	// position segment
	AttachSegment(m_positionSegment, this, vertCont);

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
	DetachSegment(m_positionSegment);
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
	auto segmentDataOpt = m_positionSegment->GetSegmentData();
	if (!segmentDataOpt.has_value()) return;

	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();
	if (!playerPosCheat->IsEnabled()) {
		spdlog::error("PlayerPosMenu::Teleport: PlayerPosCheat is not enabled");
		return;
	}

	playerPosCheat->Teleport(segmentDataOpt->position, [this](bool success) {
		m_positionSegment->SetDoUpdatePosition(true);

		if(!success) {
			ToastManager::GetInstance()->Show({ L"Teleport failed", ToastType::Error });
		}
	});
}

#include "player-pos-menu.h"
#include "../root.h"
#include "../../../converters/yes-no.h"
#include "../segments/position-segment.h"

ModMenuModule::PlayerPosMenu::PlayerPosMenu()
{
	m_positionSegment = CreateSegment<PositionSegment>("Segment_PlayerPosMenu_PositionSegment");
	m_positionSegment->SetDoUpdateFromPlayer(true);
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

	// live update position
	UiModule::Text* updatePosText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_updatePosController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		updatePosText,
		[this]() {
			return m_positionSegment->GetDoUpdateFromPlayer();
		},
		std::vector<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{L"Update position: #", L"#"}
	);
	m_updatePosController->SetConverter<YesNoConverter>();
	m_updatePosController->SetCustomSaveCallback([this](bool newValue) {
		m_positionSegment->SetDoUpdateFromPlayer(newValue);
	});

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
	if (!segmentDataOpt) return;

	PlayerPosCheat* playerPosCheat = ModMenuModule::RootModule::GetInstance()->GetCheat<PlayerPosCheat>();
	if (!playerPosCheat) return;

	playerPosCheat->Teleport(
		segmentDataOpt->x,
		segmentDataOpt->y,
		segmentDataOpt->z
	);

	m_positionSegment->SetDoUpdateFromPlayer(true);
}

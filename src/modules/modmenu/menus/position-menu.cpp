#include "position-menu.h"
#include "../root.h"

ModMenuModule::PositionMenu::PositionMenu(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey)
{
	m_posRotSegment = CreateSegment<PositionRotationSegment>(positionId, persistenceKey);
}

ModMenuModule::PositionMenu::~PositionMenu()
{

}

bool ModMenuModule::PositionMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Position#", vertCont);
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// position rotation segment
	AttachSegment(m_posRotSegment, this, vertCont);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PositionMenu::Detach()
{
	DetachSegment(m_posRotSegment);
	DestroyMenu();
}

void ModMenuModule::PositionMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::PositionMenu::OnHide()
{
	SetSegmentsVisible(false);
}

void ModMenuModule::PositionMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == 0) {
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
	}
}

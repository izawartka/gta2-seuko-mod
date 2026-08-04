#include "position-menu.h"
#include "../root.h"

ModMenuModule::PositionMenu::PositionMenu(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey)
{
	m_posSegment = CreateSegment<PositionSegment>(positionId, persistenceKey);
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

	// position segment
	AttachSegment(m_posSegment, this, vertCont);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PositionMenu::Detach()
{
	DetachSegment(m_posSegment);
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

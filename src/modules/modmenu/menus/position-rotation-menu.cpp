#include "position-rotation-menu.h"
#include "../root.h"

ModMenuModule::PositionRotationMenu::PositionRotationMenu(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey)
{
	m_posRotSegment = CreateSegment<PositionRotationSegment>(positionId, persistenceKey);
}

ModMenuModule::PositionRotationMenu::~PositionRotationMenu()
{

}

bool ModMenuModule::PositionRotationMenu::Attach()
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

void ModMenuModule::PositionRotationMenu::Detach()
{
	DetachSegment(m_posRotSegment);
	DestroyMenu();
}

void ModMenuModule::PositionRotationMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::PositionRotationMenu::OnHide()
{
	SetSegmentsVisible(false);
}

void ModMenuModule::PositionRotationMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == 0) {
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
	}
}

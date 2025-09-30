#include "menu-base.h"
#include "root.h"

bool ModMenuModule::MenuBase::Attach()
{
	UiModule::Component* vertCont = nullptr;
	CreateMenu(L"", vertCont);
	return true;
}

void ModMenuModule::MenuBase::Detach()
{
	DestroyMenu();
}

void ModMenuModule::MenuBase::CreateMenu(std::wstring title, UiModule::Component*& vertCont)
{
	assert(!m_mainComponent && !m_menuController, "Menu already created");

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuOptions options = RootModule::GetInstance()->GetOptions();

	UiModule::Margin* mainCont = uiRoot->AddComponent<UiModule::Margin>(nullptr, options.menuX, options.menuY);
	mainCont->SetVisible(false);
	m_mainComponent = static_cast<UiModule::Component*>(mainCont);

	vertCont = uiRoot->AddComponent<UiModule::VertCont>(mainCont);
	UiModule::Margin* titleMargin = uiRoot->AddComponent<UiModule::Margin>(vertCont, options.menuHeaderMarginX, options.menuHeaderMarginY);
	UiModule::Text* titleText = uiRoot->AddComponent<UiModule::Text>(titleMargin, title, options.menuHeaderTextSize);

	m_menuController = uiRoot->AddController<UiModule::MenuController>(options.menuControllerOptions);

	m_menuController->SetOnActionCallback([this](UiModule::Selectable* item, UiModule::MenuItemId id) {
		this->OnMenuAction(item, id);
	});
}

void ModMenuModule::MenuBase::DestroyMenu()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	if (m_menuController) {
		uiRoot->RemoveController(m_menuController);
		m_menuController = nullptr;
	}
	if (m_mainComponent) {
		uiRoot->RemoveComponent(m_mainComponent, true);
		m_mainComponent = nullptr;
	}
}

void ModMenuModule::MenuBase::SetVisible(bool visible)
{
	m_visible = visible;

	if (visible) {
		OnShow();
	}

	if (m_menuController) {
		m_menuController->SetActive(visible);
	}

	if (m_mainComponent) {
		m_mainComponent->SetVisible(visible);
	}

	if (!visible) {
		OnHide();
	}
}

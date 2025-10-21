#include "menu-base.h"
#include "root.h"
#include "persistence-manager.h"

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

void ModMenuModule::MenuBase::ApplyIndexPersistence(std::string key)
{
	m_selectedItemPersistenceKey = key;
	if(!m_menuController || key.empty()) {
		return;
	}
	ModMenuModule::PersistenceManager* persistence = ModMenuModule::PersistenceManager::GetInstance();
	int savedIndex = persistence->Load<int>(key, -1);
	if (savedIndex == -1) {
		return;
	}
	m_menuController->SetIndex(savedIndex);
}

void ModMenuModule::MenuBase::DestroyMenu()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	if (m_menuController) {
		if(!m_selectedItemPersistenceKey.empty()) {
			ModMenuModule::PersistenceManager* persistence = ModMenuModule::PersistenceManager::GetInstance();
			int currentIndex = m_menuController->GetIndex();
			persistence->Save<int>(m_selectedItemPersistenceKey, currentIndex);
		}

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
	if (m_visible == visible) return;
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

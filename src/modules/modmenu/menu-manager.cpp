#include "menu-manager.h"
#include "root.h"

ModMenuModule::MenuManager* ModMenuModule::MenuManager::m_instance = nullptr;

ModMenuModule::MenuManager* ModMenuModule::MenuManager::GetInstance() {
	assert(m_instance, "MenuManager not initialized!");
	return m_instance;
}

ModMenuModule::MenuManager::MenuManager() {
	assert(!m_instance, "MenuManager instance already exists");
	m_instance = this;
}

ModMenuModule::MenuManager::~MenuManager() {
	ClearMenus();
	m_instance = nullptr;
}

void ModMenuModule::MenuManager::RemoveLastMenu()
{
	if (m_menus.empty()) {
		spdlog::warn("Attempted to remove menu from empty stack");
		return;
	}
	MenuBase* backMenu = m_menus.back().get();
	backMenu->SetVisible(false);
	backMenu->Detach();
	m_menus.pop_back();

	if (m_menus.empty()) {
		return;
	}

	ModMenuModule::MenuBase* menu = m_menus.back().get();
	menu->Attach();
	menu->SetVisible(m_visible);
}

void ModMenuModule::MenuManager::ClearMenus()
{
	while (!m_menus.empty()) {
		MenuBase* backMenu = m_menus.back().get();
		backMenu->SetVisible(false);
		backMenu->Detach();
		m_menus.pop_back();
	}
}

void ModMenuModule::MenuManager::SetVisible(bool visible)
{
	if (visible == m_visible) {
		return;
	}
	m_visible = visible;

	if (m_menus.empty()) {
		return;
	}

	m_menus.back()->SetVisible(visible);
}

void ModMenuModule::MenuManager::OnKeyDown(KeyDownEvent& event)
{
	ModMenuModule::ModMenuOptions m_options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	if (event.GetKeyCode() == m_options.toggleKey) {
		SetVisible(!m_visible);
	}
}

void ModMenuModule::MenuManager::Attach() {
	AddEventListener<KeyDownEvent>(&MenuManager::OnKeyDown);
}

void ModMenuModule::MenuManager::Detach() {
	RemoveEventListener<KeyDownEvent>();
	ClearMenus();
}
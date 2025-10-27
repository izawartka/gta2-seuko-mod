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
	m_instance = nullptr;
}

void ModMenuModule::MenuManager::RemoveMenu(MenuId id)
{
	auto it = std::find(m_menuIds.begin(), m_menuIds.end(), id);
	if (it == m_menuIds.end()) {
		spdlog::warn("Attempted to remove non-existent menu with id {}", id);
		return;
	}
	m_menuIds.erase(it);
	PendingChange change = { ChangeType::Remove, id, nullptr };
	AddPendingChange(change);
}

void ModMenuModule::MenuManager::RemoveLastMenu()
{
	if (m_menuIds.empty()) {
		spdlog::warn("Attempted to remove menu from empty stack");
		return;
	}
	MenuId lastMenuId = m_menuIds.back();
	m_menuIds.pop_back();
	PendingChange change = { ChangeType::Remove, lastMenuId, nullptr };
	AddPendingChange(change);
}

void ModMenuModule::MenuManager::ClearMenus()
{
	std::vector<MenuId> menuIds = m_menuIds;
	m_menuIds.clear();
	for (MenuId id : menuIds) {
		PendingChange change = { ChangeType::Remove, id, nullptr };
		AddPendingChange(change);
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

	PendingChange change = { ChangeType::UpdateVisible, 0, nullptr };
	AddPendingChange(change);
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
	if (m_pendingChanges.size()) {
		spdlog::warn("MenuManager is being detached with {} pending changes", m_pendingChanges.size());
	}
	SetVisible(false);
	ClearMenus();
}

void ModMenuModule::MenuManager::ApplyMenuAdd(PendingChange& change)
{
	MenuBase* newMenu = static_cast<MenuBase*>(change.menu);

	auto it = std::find_if(m_menus.begin(), m_menus.end(),
		[change](const MenuStackEntry& entry) { return entry.id == change.id; });

	if (it != m_menus.end()) {
		spdlog::warn("MenuManager::ApplyMenuAdd: Menu with id {} already exists", change.id);
		return;
	}

	DetachTopMenu();
	spdlog::debug("MenuManager::ApplyMenuAdd: Adding menu with id {}", change.id);
	m_menus.push_back({ change.id, std::unique_ptr<MenuBase>(newMenu) });
}

void ModMenuModule::MenuManager::ApplyMenuRemove(PendingChange& change)
{
	auto it = std::find_if(m_menus.begin(), m_menus.end(),
		[change](const MenuStackEntry& entry) { return entry.id == change.id; });
	if (it == m_menus.end()) {
		spdlog::warn("MenuManager::ApplyMenuRemove: Menu with id {} does not exist", change.id);
		return;
	}

	if (it == m_menus.end() - 1) {
		DetachTopMenu();
	}

	spdlog::debug("MenuManager::ApplyMenuRemove: Removing menu with id {}", change.id);
	m_menus.erase(it);
}

void ModMenuModule::MenuManager::DetachTopMenu()
{
	if (m_menus.empty()) {
		return;
	}

	MenuStackEntry& topMenu = m_menus.back();
	if (m_topMenuAttached) {
		spdlog::debug("MenuManager::DetachTopMenu: Detaching top menu with id {}", topMenu.id);
		topMenu.menu->Detach();
		m_topMenuAttached = false;
	}
}

void ModMenuModule::MenuManager::ProcessPendingChanges()
{
	if (m_pendingChanges.empty()) {
		return;
	}

	m_processingChanges = true;

	MenuStackEntry* previousTopMenu = m_menus.empty() ? nullptr : &m_menus.back();
	if (previousTopMenu && previousTopMenu->menu->IsVisible()) {
		spdlog::debug("MenuManager::ProcessPendingChanges: Hiding top menu with id {}", previousTopMenu->id);
		previousTopMenu->menu->SetVisible(false);
		m_topMenuVisible = false;
	}

	while (!m_pendingChanges.empty()) {
		PendingChange change = m_pendingChanges.front();
		m_pendingChanges.pop();
		switch (change.type) {
		case ChangeType::Add:
			ApplyMenuAdd(change);
			break;
		case ChangeType::Remove:
			ApplyMenuRemove(change);
			break;
		case ChangeType::UpdateVisible:
			break;
		default:
			spdlog::warn("MenuManager::ProcessPendingChanges: Unknown change type");
			break;
		}
	}

	MenuStackEntry* newTopMenu = m_menus.empty() ? nullptr : &m_menus.back();

	if(!m_topMenuAttached && newTopMenu) {
		spdlog::debug("MenuManager::ProcessPendingChanges: Attaching top menu with id {}", newTopMenu->id);
		newTopMenu->menu->Attach();
		m_topMenuAttached = true;
	}

	if(!m_topMenuVisible && newTopMenu && m_visible) {
		spdlog::debug("MenuManager::ProcessPendingChanges: Setting top menu with id {} visible", newTopMenu->id);
		newTopMenu->menu->SetVisible(true);
		m_topMenuVisible = true;
	}

	m_processingChanges = false;

	if(!m_pendingChanges.empty()) {
		ProcessPendingChanges();
	}
}

void ModMenuModule::MenuManager::AddPendingChange(const PendingChange& change)
{
	m_pendingChanges.push(change);
	if (!m_processingChanges) {
		ProcessPendingChanges();
	}
}

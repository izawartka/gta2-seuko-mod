#include "menu.h"
#include "var-text.h"

UiModule::MenuController::MenuController(const MenuControllerOptions& options)
	: StandardBindsSupport::StandardBindsSupport(options.keyBindOptions)
{
	m_options = options;
}

UiModule::MenuController::~MenuController()
{
	SetActive(false);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	while (!m_items.empty()) {
		MenuItem& menuItem = m_items.back();
		if (menuItem.controller) {
			uiRoot->RemoveController(static_cast<Controller*>(menuItem.controller));
		}
		m_items.pop_back();
	}
}

UiModule::MenuItemId UiModule::MenuController::AddItem(Selectable* item)
{
	if (!item) {
		spdlog::error("Attempted to add null item to MenuController");
		return -1;
	}

	MenuItem menuItem = {};
	menuItem.id = m_nextItemId++;
	menuItem.item = item;
	menuItem.groupId = m_currentGroupId;

	if(m_nextAddedItemIndex > m_items.size()) {
		spdlog::warn("MenuController::AddItem: Invalid next added item index {}", m_nextAddedItemIndex);
		m_nextAddedItemIndex = m_items.size();
	}
	
	m_items.insert(m_items.begin() + m_nextAddedItemIndex, menuItem);
	if (m_currentIndex >= m_nextAddedItemIndex) {
		SetIndex(m_currentIndex + 1);
	}
	m_nextAddedItemIndex++;

	if (m_items.size() == 1) {
		SetIndex(0);
	}

	return m_nextItemId - 1;
}

std::vector<UiModule::MenuItemId> UiModule::MenuController::AddItems(const std::vector<Selectable*>& items)
{
	std::vector<UiModule::MenuItemId> ids;
	for (auto* item : items) {
		ids.push_back(AddItem(item));
	}
	return ids;
}

void UiModule::MenuController::RemoveItem(MenuItemId id)
{
	MenuItem* item = GetItemById(id);
	if (!item) return;

	if(item->controller) {
		spdlog::error("MenuController::RemoveItem: Item with id {} has a controller managed by this menu. Remove the controller first.", id);
		return;
	}

	size_t itemIndex = std::distance(m_items.data(), item);

	item->item->SetSelected(false);
	m_items.erase(m_items.begin() + itemIndex);

	if(m_items.empty()) {
		m_currentIndex = -1;
		return;
	}

	if (itemIndex <= m_currentIndex && m_currentIndex > 0) {
		SetIndex(m_currentIndex - 1);
	}
	else {
		UpdateIndex();
	}
}

void UiModule::MenuController::AddItemController(MenuItemId id, MenuItemController* controller)
{
	MenuItem* menuItem = GetItemById(id);
	if (!menuItem) return;
	if (menuItem->controller != nullptr) {
		spdlog::error("MenuController::AddItemController: Item with id {} already has a controller", id);
		return;
	}
	menuItem->controller = controller;
	controller->SetEditStopCallback(std::bind(&MenuController::OnItemEditStop, this));
}

void UiModule::MenuController::AddLatestItemController(MenuItemController* controller)
{
	if (m_nextItemId == 0) {
		spdlog::error("MenuController::AddLatestItemController: No items have been added yet");
		return;
	}
	AddItemController(m_nextItemId - 1, controller);
}

UiModule::MenuItemController* UiModule::MenuController::RemoveItemController(MenuItemId id)
{
	MenuItem* menuItem = GetItemById(id);
	if (!menuItem) return nullptr;

	MenuItemController* controller = menuItem->controller;
	if (!controller) {
		spdlog::error("MenuController::RemoveItemController: Item with id {} has no controller", id);
		return nullptr;
	}

	if (m_activeController == controller) SetActiveController(nullptr);

	menuItem->controller = nullptr;
	return controller;
}

void UiModule::MenuController::DeleteItemController(MenuItemId id)
{
	MenuItemController* controller = RemoveItemController(id);
	if (controller == nullptr) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController((Controller*)controller);
}

void UiModule::MenuController::DeleteGroupItems(MenuItemGroupId groupId)
{
	for (size_t i = m_items.size(); i-- > 0;) {
		MenuItem& menuItem = m_items[i];
		if (menuItem.groupId == groupId) {
			if (menuItem.controller) {
				UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
				uiRoot->RemoveController(static_cast<Controller*>(menuItem.controller));
			}
			menuItem.item->SetSelected(false);
			m_items.erase(m_items.begin() + i);
			if (i < m_nextAddedItemIndex) {
				m_nextAddedItemIndex--;
			}
			if (i <= m_currentIndex && m_currentIndex > 0) {
				SetIndex(m_currentIndex - 1);
			}
		}
	}
}

void UiModule::MenuController::DeleteCurrentGroupItems()
{
	DeleteGroupItems(m_currentGroupId);
}

void UiModule::MenuController::SetItemsWatching(bool watching)
{
	if (m_itemsWatching == watching) return;
	m_itemsWatching = watching;

	for (auto& menuItem : m_items) {
		if (menuItem.controller) {
			menuItem.controller->SetWatching(watching);
		}
	}
}

void UiModule::MenuController::SetActive(bool active)
{
	if(active == m_active) {
		return;
	}

	m_active = active;
	SetActiveMenuControl(active);
	SetActiveController(nullptr);
}

void UiModule::MenuController::SetIndex(size_t index)
{
	if (m_currentIndex == index) return;
	if (index < 0 || index >= m_items.size()) {
		spdlog::warn("Attempted to set invalid index in MenuController");
		return;
	}
	SetActiveController(nullptr);
	m_currentIndex = index;
	UpdateIndex();
}

void UiModule::MenuController::Next()
{
	if(!m_options.loop && m_currentIndex + 1 >= m_items.size()) {
		return;
	}

	if(m_items.empty()) {
		return;
	}

	SetIndex((m_currentIndex + 1) % m_items.size());
}

void UiModule::MenuController::Previous()
{
	if(!m_options.loop && m_currentIndex == 0) {
		return;
	}

	if(m_items.empty()) {
		return;
	}

	SetIndex((m_currentIndex - 1 + m_items.size()) % m_items.size());
}

void UiModule::MenuController::Action()
{
	if (!m_activeMenuControl) return;
	if (m_currentIndex < 0 && m_currentIndex >= m_items.size()) return;

	MenuItem& menuItem = m_items[m_currentIndex];
	if (menuItem.controller) SetActiveController(menuItem.controller);
	if (m_onAction) m_onAction(menuItem.item, menuItem.id);
}

void UiModule::MenuController::OnItemEditStop()
{
	m_activeController = nullptr;
	if (m_active) SetActiveMenuControl(true);
}

void UiModule::MenuController::OnKeyDown(KeyDownEvent& event)
{
	if (!m_activeMenuControl) {
		return;
	}

	KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyDownEvent(event);

	if (IsPrevKey(key)) {
		Previous();
	}
	else if (IsNextKey(key)) {
		Next();
	}
	else if (IsActionKey(key)) {
		Action();
	}
}

UiModule::MenuController::MenuItem* UiModule::MenuController::GetItemById(MenuItemId id)
{
	auto it = std::find_if(
		m_items.begin(),
		m_items.end(),
		[id](const MenuItem& menuItem) {
			return menuItem.id == id;
		}
	);

	if (it == m_items.end()) {
		spdlog::error("MenuController::GetItemById: No item with id {}", id);
		return nullptr;

	}

	return &(*it);
}

void UiModule::MenuController::SetActiveMenuControl(bool active)
{
	if (m_activeMenuControl == active) return;
	m_activeMenuControl = active;

	if(active) AddEventListener<KeyDownEvent>(&MenuController::OnKeyDown);
	else RemoveEventListener<KeyDownEvent>();

	if (m_options.hideMarkerNonActive) {
		UpdateIndex();
	}
}

void UiModule::MenuController::SetActiveController(MenuItemController* controller)
{
	if (m_activeController == controller) return;
	if (m_activeController != nullptr) {
		m_activeController->SetEditing(false);
	}
	m_activeController = controller;
	if (controller != nullptr) {
		SetActiveMenuControl(false);
		controller->SetEditing(true);
	}
}

void UiModule::MenuController::UpdateIndex()
{
	bool hideIndex = !m_activeMenuControl && m_options.hideMarkerNonActive;
	for (size_t i = 0; i < m_items.size(); ++i) {
		m_items[i].item->SetSelected(i == m_currentIndex && !hideIndex);
	}
}

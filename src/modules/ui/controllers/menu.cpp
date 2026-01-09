#include "menu.h"

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
			uiRoot->RemoveController(menuItem.controller);
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
		m_currentIndex++;
	}
	m_nextAddedItemIndex++;
	UpdateIndex();

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

void UiModule::MenuController::DeleteItem(MenuItemId id)
{
	MenuItem* item = GetItemById(id);
	if (!item) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();

	if(item->controller) {
		if (m_activeController == item->controller) SetActiveController(nullptr);
		if(m_itemsWatching) item->controller->SetWatching(false);
		uiRoot->RemoveController(item->controller);
	}

	uiRoot->RemoveComponent(item->item, true);
	size_t itemIndex = std::distance(m_items.data(), item);
	m_items.erase(m_items.begin() + itemIndex);

	if(m_items.empty()) {
		m_currentIndex = -1;
		return;
	}

	if (itemIndex == m_currentIndex) {
		m_currentIndex--;
		if (m_currentIndex < 0) m_currentIndex = 0;
		UpdateIndex();
	}
	else if (itemIndex < m_currentIndex) {
		m_currentIndex--;
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
	if (m_itemsWatching) controller->SetWatching(true);
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
	controller->SetEditStopCallback(nullptr);

	menuItem->controller = nullptr;
	return controller;
}

void UiModule::MenuController::DeleteItemController(MenuItemId id)
{
	MenuItemController* controller = RemoveItemController(id);
	if (controller == nullptr) return;

	if(m_itemsWatching) controller->SetWatching(false);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController(controller);
}

size_t UiModule::MenuController::GetItemIndex(MenuItemId id) const
{
	for (const auto& menuItem : m_items) {
		if (menuItem.id == id) {
			return &menuItem - m_items.data();
		}
	}
	return -1;
}

void UiModule::MenuController::SetCurrentGroupId(MenuItemGroupId groupId)
{
	if (groupId >= m_nextFreeGroupId || groupId == -1) {
		spdlog::error("UiModule::MenuController::SetCurrentGroupId: Invalid group ID: {}", groupId);
		return;
	}

	m_currentGroupId = groupId;
}

void UiModule::MenuController::DeleteGroupItems(MenuItemGroupId groupId)
{
	if (groupId == -1) {
		spdlog::error("UiModule::MenuController::DeleteGroupItems: Cannot delete items from group -1");
		return;
	}

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();

	for (size_t i = 0; i < m_items.size(); i++) {
		MenuItem& menuItem = m_items.at(i);
		if (menuItem.groupId != groupId) continue;

		if (menuItem.controller) {
			if (m_activeController == menuItem.controller) SetActiveController(nullptr);
			if (m_itemsWatching) menuItem.controller->SetWatching(false);
			uiRoot->RemoveController(static_cast<Controller*>(menuItem.controller));
		}
		uiRoot->RemoveComponent(menuItem.item, true);

		size_t itemIndex = &menuItem - m_items.data();
		if (itemIndex < m_nextAddedItemIndex) {
			m_nextAddedItemIndex--;
		}
		if (itemIndex <= m_currentIndex && m_currentIndex > 0) {
			m_currentIndex--;
		}

		m_items.erase(m_items.begin() + itemIndex);
		i--;
	}

	UpdateIndex();
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
	if (!m_activeMenuControl) return;

	KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyboardEvent(event);

	if (IsActionKey(key)) Action();
}

void UiModule::MenuController::OnKeyDownRepeat(KeyDownRepeatEvent& event)
{
	if (!m_activeMenuControl) return;

	KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyboardEvent(event);

	if (IsPrevKey(key)) Previous();
	else if (IsNextKey(key)) Next();
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

	if (active) {
		AddEventListener<KeyDownEvent>(&MenuController::OnKeyDown);
		AddEventListener<KeyDownRepeatEvent>(&MenuController::OnKeyDownRepeat);
	}
	else {
		RemoveEventListener<KeyDownEvent>();
		RemoveEventListener<KeyDownRepeatEvent>();
	}

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

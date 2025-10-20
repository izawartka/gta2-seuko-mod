#include "menu.h"
#include "var-text.h"

UiModule::MenuController::MenuController(MenuControllerOptions options)
{
	m_options = options;
}

UiModule::MenuController::~MenuController()
{

}

UiModule::MenuItemId UiModule::MenuController::AddItem(Selectable* item)
{
	if (!item) {
		spdlog::error("Attempted to add null item to MenuController");
		return -1;
	}
	m_items.push_back(item);
	if (m_items.size() == 1) {
		SetIndex(0);
	}
	return m_items.size() - 1;
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
	if (id < 0 || id >= m_items.size()) {
		spdlog::error("Attempted to remove invalid item from MenuController");
		return;
	}
	m_items.erase(m_items.begin() + id);

	MenuItemId index = m_currentIndex;
	if (m_currentIndex >= id && m_currentIndex > 0) {
		index = m_currentIndex - 1;
	}

	SetIndex(index);
}

void UiModule::MenuController::SetActive(bool active)
{
	if(active == m_active) {
		return;
	}

	m_active = active;
	if (active) {
		AddEventListener<KeyDownEvent>(&MenuController::OnKeyDown);
	} else {
		RemoveEventListener<KeyDownEvent>();
	}

	if (m_options.hideMarkerNonActive) {
		SetIndex(m_currentIndex);
	}
}

void UiModule::MenuController::SetIndex(MenuItemId index)
{
	if (index < 0 || index >= m_items.size()) {
		spdlog::error("Attempted to set invalid index in MenuController");
		return;
	}
	m_currentIndex = index;
	bool hideIndex = !m_active && m_options.hideMarkerNonActive;
	for (size_t i = 0; i < m_items.size(); ++i) {
		m_items[i]->SetSelected(i == m_currentIndex && !hideIndex);
	}
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
	if (m_onAction && m_currentIndex >= 0 && m_currentIndex < m_items.size()) {
		m_onAction(m_items[m_currentIndex], m_currentIndex);
	}
}

void UiModule::MenuController::OnKeyDown(KeyDownEvent& event)
{
	if (!m_active) {
		return;
	}

	Game::KeyCode key = event.GetKeyCode();

	if( key == m_options.keyPrev ) {
		Previous();
	} else if( key == m_options.keyNext ) {
		Next();
	} else if( key == m_options.keyAction ) {
		Action();
	}
}

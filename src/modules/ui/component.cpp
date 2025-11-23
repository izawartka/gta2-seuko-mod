#include "component.h"

UiModule::Component::~Component()
{
	while(!m_children.empty()) {
		RemoveChild(m_children.back());
	}

	if(m_parent) {
		m_parent->RemoveChild(this);
	}
}

void UiModule::Component::DrawIfVisible()
{
	if (!m_visibleInherited) {
		return;
	}
	Draw();
}

void UiModule::Component::AddChild(Component* child)
{
	if (!child) {
		spdlog::error("Attempted to add null child to component");
		return;
	}

	child->m_parent = this;
	m_children.push_back(child);
	UpdateChildrenPos();
	child->UpdateChildrenVisibility();
}

void UiModule::Component::RemoveChild(Component* child)
{
	if (!child) {
		spdlog::error("Attempted to remove null child from component");
		return;
	}
	auto it = std::find(m_children.begin(), m_children.end(), child);
	if (it == m_children.end()) {
		spdlog::error("Attempted to remove a child that is not a child of this component");
		return;
	}
	m_children.erase(it);
	child->m_parent = nullptr;
	child->SetPosition(0, 0);
	UpdateChildrenPos();
	child->UpdateChildrenVisibility();
}

void UiModule::Component::UpdateChildrenPos()
{
	for (auto child : m_children) {
		child->SetPosition(GetRect().x, GetRect().y);
	}
}

void UiModule::Component::SetPosition(Game::SCR_f x, Game::SCR_f y)
{
	m_rect.x = x;
	m_rect.y = y;
	UpdateChildrenPos();
}

void UiModule::Component::OnChildSizeUpdate(Component* child)
{
	Rect prevRect = GetRect();
	UpdateChildrenPos();
	const Rect& rect = GetRect();
	if (m_parent && (rect.width != prevRect.width || rect.height != prevRect.height)) {
		m_parent->OnChildSizeUpdate(this);
	}
}

void UiModule::Component::SetVisible(bool visible)
{
	if (m_visible == visible) {
		return;
	}
	m_visible = visible;
	UpdateChildrenVisibility();
}

void UiModule::Component::UpdateSize(Game::SCR_f* width, Game::SCR_f* height)
{
	Rect prevRect = m_rect;
	if(width) m_rect.width = *width;
	if(height) m_rect.height = *height;
	if (m_parent && (m_rect.width != prevRect.width || m_rect.height != prevRect.height)) {
		m_parent->OnChildSizeUpdate(this);
	}
}

void UiModule::Component::UpdateChildrenVisibility()
{
	bool parentVisible = m_parent ? m_parent->m_visibleInherited : true;
	m_visibleInherited = m_visible && parentVisible;
	for (auto child : m_children) {
		child->UpdateChildrenVisibility();
	}
}

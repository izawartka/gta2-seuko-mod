#include "overridepos.h"

UiModule::OverridePos::OverridePos(Component* parent, std::optional<Game::SCR_f> x, std::optional<Game::SCR_f> y)
{
	m_x = x;
	m_y = y;

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::OverridePos::UpdateChildrenPos()
{
	Game::SCR_f maxWidth = 0;
	Game::SCR_f maxHeight = 0;

	for (auto child : m_children) {
		child->SetPosition(
			m_x.has_value() ? m_x.value() : GetRect().x,
			m_y.has_value() ? m_y.value() : GetRect().y
		);

		const Rect& childRect = child->GetRect();
		maxWidth = std::max(maxWidth, childRect.width);
		maxHeight = std::max(maxHeight, childRect.height);
	}

	UpdateSize(
		m_x.has_value() ? nullptr : &maxWidth,
		m_y.has_value() ? nullptr : &maxHeight
	);
}

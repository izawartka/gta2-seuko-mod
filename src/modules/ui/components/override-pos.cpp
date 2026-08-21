#include "override-pos.h"

UiModule::OverridePos::OverridePos(Component* parent, std::optional<OverridePosCoord> x, std::optional<OverridePosCoord> y)
{
	m_x = x;
	m_y = y;

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::OverridePos::UpdateChildrenPos()
{
	Game::SCR_f x = m_x.has_value() ? Utils::ScreenAlignHorizontal(m_x->align, m_x->offset) : GetRect().x;
	Game::SCR_f y = m_y.has_value() ? Utils::ScreenAlignVertical(m_y->align, m_y->offset) : GetRect().y;

	Game::SCR_f maxWidth = 0;
	Game::SCR_f maxHeight = 0;

	for (auto child : m_children) {
		const Rect& childRect = child->GetRect();
		maxWidth = std::max(maxWidth, childRect.width);
		maxHeight = std::max(maxHeight, childRect.height);
	}

	for (auto child : m_children) {
		Game::SCR_f childX = m_x.has_value() ?
			x - Utils::ChildAlign(m_x->align, maxWidth) :
			x;

		Game::SCR_f childY = m_y.has_value() ?
			y - Utils::ChildAlign(m_y->align, maxHeight) :
			y;

		child->SetPosition(childX, childY);
	}

	UpdateSize(
		m_x.has_value() ? nullptr : &maxWidth,
		m_y.has_value() ? nullptr : &maxHeight
	);
}

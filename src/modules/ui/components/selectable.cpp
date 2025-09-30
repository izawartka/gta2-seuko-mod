#include "selectable.h"

UiModule::Selectable::Selectable(Component* parent, SelectableOptions options)
{
	m_options = options;
	m_rect.width = options.markerOffsetX;
	m_rect.height = options.markerScale * 20;

	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::Selectable::Draw()
{
	if(!m_options.selected) {
		return;
	}

	Game::S4_ENUM1 s4enum2 = (Game::S4_ENUM1)2;
	Game::Functions::DrawGTAText(
		(WCHAR*)m_options.markerText.c_str(),
		m_rect.x,
		m_rect.y + m_options.markerAdditionalOffsetY,
		1,
		m_options.markerScale,
		&s4enum2,
		0,
		Game::SPRITE_INVISIBILITY_VISIBLE,
		0
	);
}

void UiModule::Selectable::UpdateChildrenPos()
{
	Game::SCR_f maxWidth = m_options.markerOffsetX;
	Game::SCR_f maxHeight = 0;
	for (auto child : m_children) {
		child->SetPosition(m_rect.x + m_options.markerOffsetX, m_rect.y);
		Rect childRect = child->GetRect();
		maxWidth = max(maxWidth, m_options.markerOffsetX + childRect.width);
		maxHeight = max(maxHeight, childRect.height);

	}

	UpdateSize(&maxWidth, &maxHeight);
}

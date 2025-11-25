#include "horcentered.h"

UiModule::HorCentered::HorCentered(Component* parent)
{
	if (parent) {
		parent->AddChild(this);
	}
}

void UiModule::HorCentered::UpdateChildrenPos()
{
	Game::SCR_f maxHeight = 0;

	for (auto child : m_children) {
		const Rect& childRect = child->GetRect();
		Game::SCR_f childX = (Game::Utils::FromFloat(640.0f) - childRect.width) / 2;
		child->SetPosition(childX, m_rect.y);
		maxHeight = std::max(maxHeight, childRect.height);
	}

	UpdateSize(nullptr, &maxHeight);
}

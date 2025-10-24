#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	class HorCont : public Component {
	public:
		HorCont(Component* parent) {
			if (parent) {
				parent->AddChild(this);
			}
		}

		virtual void UpdateChildrenPos() override {
			Game::SCR_f currentX = m_rect.x;
			Game::SCR_f maxHeight = 0;
			for (auto child : m_children) {
				child->SetPosition(currentX, m_rect.y);
				Rect childRect = child->GetRect();
				currentX += childRect.width;
				maxHeight = std::max(maxHeight, childRect.height);
			}

			Game::SCR_f newWidth = currentX - m_rect.x;
			UpdateSize(&newWidth, &maxHeight);
		}
	};
}
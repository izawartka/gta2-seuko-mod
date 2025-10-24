#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	class VertCont : public Component {
	public:
		VertCont(Component* parent) {
			if (parent) {
				parent->AddChild(this);
			}
		}

		virtual void UpdateChildrenPos() override {
			Game::SCR_f currentY = m_rect.y;
			Game::SCR_f maxWidth = 0;
			for (auto child : m_children) {
				child->SetPosition(m_rect.x, currentY);
				Rect childRect = child->GetRect();
				currentY += childRect.height;
				maxWidth = std::max(maxWidth, childRect.width);
			}

			Game::SCR_f newHeight = currentY - m_rect.y;
			UpdateSize(&maxWidth, &newHeight);
		}
	};
}
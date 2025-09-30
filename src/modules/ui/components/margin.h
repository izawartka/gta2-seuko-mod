#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	class Margin : public Component {
	public:
		Margin(Component* parent, Game::SCR_f marginX, Game::SCR_f marginY) {
			m_marginX = marginX;
			m_marginY = marginY;
			m_rect.width = marginX*2;
			m_rect.height = marginY*2;

			if (parent) {
				parent->AddChild(this);
			}
		}

		virtual void UpdateChildrenPos() override {
			Game::SCR_f maxWidth = 0;
			Game::SCR_f maxHeight = 0;

			for (auto child : m_children) {
				child->SetPosition(m_rect.x + m_marginX, m_rect.y + m_marginY);
				Rect childRect = child->GetRect();
				maxWidth = max(maxWidth, childRect.width);
				maxHeight = max(maxHeight, childRect.height);
			}

			Game::SCR_f newWidth = maxWidth + m_marginX * 2;
			Game::SCR_f newHeight = maxHeight + m_marginY * 2;
			UpdateSize(&newWidth, &newHeight);
		}

		void SetMargin(Game::SCR_f marginX, Game::SCR_f marginY) {
			m_marginX = marginX;
			m_marginY = marginY;
			UpdateChildrenPos();
		}
		Game::SCR_f GetMarginX() const { return m_marginX; }
		Game::SCR_f GetMarginY() const { return m_marginY; }

	protected:
		Game::SCR_f m_marginX = 0;
		Game::SCR_f m_marginY = 0;
	};
}
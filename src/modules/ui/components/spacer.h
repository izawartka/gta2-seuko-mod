#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	class Spacer : public Component {
	public:
		Spacer(Component* parent, Game::SCR_f width, Game::SCR_f height) {
			m_rect.width = width;
			m_rect.height = height;
			if (parent) {
				parent->AddChild(this);
			}
		}

		void SetSize(Game::SCR_f width, Game::SCR_f height) {
			UpdateSize(&width, &height);
		}
		Game::SCR_f GetWidth() const { return m_rect.width; }
		Game::SCR_f GetHeight() const { return m_rect.height; }
	};
}
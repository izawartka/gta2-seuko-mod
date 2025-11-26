#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	class OverridePos : public Component {
	public:
		OverridePos(Component* parent, std::optional<Game::SCR_f> x, std::optional<Game::SCR_f> y);
		virtual void UpdateChildrenPos() override;

	private:
		std::optional<Game::SCR_f> m_x = std::nullopt;
		std::optional<Game::SCR_f> m_y = std::nullopt;
	};
}

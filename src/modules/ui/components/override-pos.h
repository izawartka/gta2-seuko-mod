#pragma once
#include "../common.h"
#include "../component.h"
#include "../utils/align.h"

namespace UiModule {
	using OverridePosAlign = Utils::Align;

	struct OverridePosCoord {
		OverridePosAlign align = OverridePosAlign::Start;
		Game::SCR_f offset = 0;

		OverridePosCoord(OverridePosAlign align, Game::SCR_f offset = 0) {
			this->align = align;
			this->offset = offset;
		}

		// for backwards compatibility
		OverridePosCoord(Game::SCR_f offset) {
			this->offset = offset;
		}
	};

	class OverridePos : public Component {
	public:
		OverridePos(Component* parent, std::optional<OverridePosCoord> x, std::optional<OverridePosCoord> y);
		virtual void UpdateChildrenPos() override;

	private:
		std::optional<OverridePosCoord> m_x = std::nullopt;
		std::optional<OverridePosCoord> m_y = std::nullopt;
	};
}

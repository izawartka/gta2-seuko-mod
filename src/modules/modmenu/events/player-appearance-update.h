#pragma once
#include "../common.h"

namespace ModMenuModule {
	class PlayerAppearanceUpdateEvent : public Core::EventBase {
	public:
		PlayerAppearanceUpdateEvent(const std::optional<Game::PED_REMAP>& remap, const std::optional<Game::PED_GRAPHIC_TYPE>& graphicType)
			: m_remap(remap), m_graphicType(graphicType) {}

		const std::optional<Game::PED_REMAP>& GetRemap() const {
			return m_remap;
		}

		const std::optional<Game::PED_GRAPHIC_TYPE>& GetGraphicType() const {
			return m_graphicType;
		}

	private:
		std::optional<Game::PED_REMAP> m_remap;
		std::optional<Game::PED_GRAPHIC_TYPE> m_graphicType;
	};
}

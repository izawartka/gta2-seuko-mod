#pragma once
#include "../common.h"

namespace ModMenuModule {
	class PlayerPosUpdateEvent : public Core::EventBase {
	public:
		PlayerPosUpdateEvent(const std::optional<Game::SCR_Vector3>& position)
			: m_position(position) {
		};

		const std::optional<Game::SCR_Vector3>& GetPosition() const {
			return m_position;
		}

	private:
		std::optional<Game::SCR_Vector3> m_position;
	};
}

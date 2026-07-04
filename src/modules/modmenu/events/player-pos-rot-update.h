#pragma once
#include "../common.h"

namespace ModMenuModule {
	class PlayerPosRotUpdateEvent : public Core::EventBase {
	public:
		PlayerPosRotUpdateEvent(const std::optional<Game::SCR_Vector3>& position, const std::optional<short>& rotation)
			: m_position(position), m_rotation(rotation) {}

		const std::optional<Game::SCR_Vector3>& GetPosition() const {
			return m_position;
		}

		const std::optional<short>& GetRotation() const {
			return m_rotation;
		}

	private:
		std::optional<Game::SCR_Vector3> m_position;
		std::optional<short> m_rotation;
	};
}

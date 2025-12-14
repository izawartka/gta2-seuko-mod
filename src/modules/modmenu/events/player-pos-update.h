#pragma once
#include "../common.h"
#include "../cheats/player-pos.h"

namespace ModMenuModule {
	class PlayerPosUpdateEvent : public Core::EventBase {
	public:
		PlayerPosUpdateEvent(std::optional<PlayerPosCheatPos> position)
			: m_position(position) {
		};

		std::optional<PlayerPosCheatPos> GetPosition() const {
			return m_position;
		}

	private:
		std::optional<PlayerPosCheatPos> m_position;
	};
}

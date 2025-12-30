#pragma once
#include "../common.h"

namespace ModMenuModule {
	class PlayerRotUpdateEvent : public Core::EventBase {
	public:
		PlayerRotUpdateEvent(const std::optional<short>& rotation)
			: m_rotation(rotation) {
		};

		const std::optional<short>& GetRotation() const {
			return m_rotation;
		}

	private:
		std::optional<short> m_rotation;
	};
}

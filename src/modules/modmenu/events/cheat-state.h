#pragma once
#include "../common.h"

namespace ModMenuModule {
	class CheatStateEvent : public Core::EventBase {
	public:
		CheatStateEvent(std::type_index typeIdx, bool enabled)
			: m_typeIndex(typeIdx), m_enabled(enabled) {
		};
		virtual ~CheatStateEvent() override {};

		std::type_index GetCheatType() const { return m_typeIndex; }
		bool IsEnabled() const { return m_enabled; }

	private:
		std::type_index m_typeIndex;
		bool m_enabled;
	};
}

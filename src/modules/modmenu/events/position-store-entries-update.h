#pragma once
#include "../common.h"

namespace ModMenuModule {
	class PositionStoreEntriesUpdateEvent : public Core::EventBase {
	public:
		PositionStoreEntriesUpdateEvent() {}
		virtual ~PositionStoreEntriesUpdateEvent() override {};
	};
}

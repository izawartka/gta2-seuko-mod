#pragma once
#include "../common.h"
#include "../cheats/last-car.h"

namespace ModMenuModule {
	class LastCarStateEvent : public Core::EventBase {
	public:
		LastCarStateEvent(LastCarState state)
			: m_state(state) {
		};
		virtual ~LastCarStateEvent() override {};

		LastCarState GetState() const { return m_state; }

	private:
		LastCarState m_state;
	};
}

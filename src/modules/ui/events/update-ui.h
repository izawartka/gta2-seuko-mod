#pragma once
#include "../common.h"

namespace UiModule {
	/*
	Dispatched before any UiModule logic is done each frame. 
	To be used by other modules that use UiModule controllers.
	*/
	class PreUpdateUIEvent : public Core::EventBase {
	public:
		PreUpdateUIEvent() {};
		virtual ~PreUpdateUIEvent() override {};
	};

	/*
	UiModule controllers update on this event.
	*/
	class UpdateUIEvent : public Core::EventBase {
	public:
		UpdateUIEvent() {};
		virtual ~UpdateUIEvent() override {};
	};
}

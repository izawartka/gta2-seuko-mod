#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../../../../events/draw-frame.h"

namespace ModMenuModule {
	class ClearScreenCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		ClearScreenCheat();
		virtual ~ClearScreenCheat();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnClearScreen(ClearScreenEvent& event);
	};
}

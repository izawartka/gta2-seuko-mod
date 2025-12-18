#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../../../../events/draw-frame.h"

namespace ModMenuModule {
	class ClearScreenCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		ClearScreenCheat();
		virtual ~ClearScreenCheat() override;
		static ClearScreenCheat* GetInstance();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnClearScreen(ClearScreenEvent& event);

		static ClearScreenCheat* m_instance;
	};
}

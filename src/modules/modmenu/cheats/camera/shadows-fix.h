#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../../../../events/scale-shadow.h"

namespace ModMenuModule {
	class ShadowsFixCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		ShadowsFixCheat();
		virtual ~ShadowsFixCheat();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnScaleShadow(ScaleShadowEvent& event);
	};
}

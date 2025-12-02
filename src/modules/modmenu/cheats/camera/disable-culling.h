#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../../../../events/culling-check.h"

namespace ModMenuModule {
	class DisableCullingCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		DisableCullingCheat();
		virtual ~DisableCullingCheat();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnCullingCheck(CullingCheckEvent& event);
	};
}

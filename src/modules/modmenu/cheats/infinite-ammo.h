#pragma once
#include "../common.h"
#include "../../../events/is-ammo-kf-call.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	class InfiniteAmmoCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		InfiniteAmmoCheat();
		virtual ~InfiniteAmmoCheat() override;
		static InfiniteAmmoCheat* GetInstance();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnIsAmmoKfCall(IsAmmoKfCallEvent& event);

		static InfiniteAmmoCheat* m_instance;
	};
}

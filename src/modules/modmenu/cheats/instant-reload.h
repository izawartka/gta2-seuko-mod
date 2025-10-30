#pragma once
#include "../common.h"
#include "../../../events/try-shoot-weapon.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	class InstantReloadCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		InstantReloadCheat();
		virtual ~InstantReloadCheat();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnTryShootWeapon(TryShootWeaponEvent& event);
	};
}

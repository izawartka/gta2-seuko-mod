#pragma once
#include "../common.h"
#include "../../../events/steering-assist.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	class DisableSteeringAssistCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		DisableSteeringAssistCheat();
		virtual ~DisableSteeringAssistCheat() override;
		static DisableSteeringAssistCheat* GetInstance();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnSteeringAssist(SteeringAssistEvent& event);

		static DisableSteeringAssistCheat* m_instance;
	};
}

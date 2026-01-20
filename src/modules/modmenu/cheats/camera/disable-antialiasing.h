#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../../../../events/renderer.h"
#include "../../../../events/renderer-antialiasing-check.h"

namespace ModMenuModule {
	class DisableAntialiasingCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		DisableAntialiasingCheat();
		virtual ~DisableAntialiasingCheat() override;
		static DisableAntialiasingCheat* GetInstance();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnRendererLoad(RendererLoadEvent& event);
		void OnAntialiasingCheck(RendererAntialiasingCheckEvent& event);

		static DisableAntialiasingCheat* m_instance;
	};
}

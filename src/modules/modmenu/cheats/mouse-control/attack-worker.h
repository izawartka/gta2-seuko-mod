#pragma once
#include "mouse-control-worker.h"
#include "../force-controls.h"

namespace ModMenuModule {
	class AttackWorker : public MouseControlWorker, public Core::EventListenerSupport {
	public:
		AttackWorker() = default;
		AttackWorker(const AttackWorker&) = delete;
		AttackWorker& operator=(const AttackWorker&) = delete;

		void Start() override;
		void Stop() override;
		bool IsRunning() const override { return m_running; }

	private:
		void OnMouseButtonDown(MouseModule::MouseButtonDownEvent& event);
		void OnMouseButtonUp(MouseModule::MouseButtonUpEvent& event);

		bool CreateControlHandle();
		void FreeControlHandle();

		void ForceUpdate() const;

		bool m_running = false;
		ForceControlsCheat::ControlHandle m_attackControlHandle = -1;
	};
}

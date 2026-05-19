#pragma once
#include "mouse-control-worker.h"
#include "../force-controls.h"

namespace ModMenuModule {
	class LeftRightWorker : public MouseControlWorker, public Core::EventListenerSupport {
	public:
		LeftRightWorker() = default;
		LeftRightWorker(const LeftRightWorker&) = delete;
		LeftRightWorker& operator=(const LeftRightWorker&) = delete;

		void Start() override;
		void Stop() override;
		bool IsRunning() const override { return m_running; }
		void Update() override;

	private:
		bool CreateControlHandles();
		void FreeControlHandles();

		static char GetDirection(float deltaAngle);

		bool m_running = false;
		ForceControlsCheat::ControlHandle m_leftControlHandle = -1;
		ForceControlsCheat::ControlHandle m_rightControlHandle = -1;
	};
}

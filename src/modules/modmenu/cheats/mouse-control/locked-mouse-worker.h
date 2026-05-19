#pragma once
#include "mouse-control-worker.h"
#include "../force-controls.h"

namespace ModMenuModule {
	class LockedMouseWorker : public MouseControlWorker, public Core::EventListenerSupport {
	public:
		LockedMouseWorker() = default;
		LockedMouseWorker(const LockedMouseWorker&) = delete;
		LockedMouseWorker& operator=(const LockedMouseWorker&) = delete;

		void Start() override;
		void Stop() override;
		bool IsRunning() const override { return m_running; }
		void Update() override;

	private:
		void OnMouseLockedMove(MouseModule::MouseLockedMoveEvent& event);

		bool m_running = false;
	};
}

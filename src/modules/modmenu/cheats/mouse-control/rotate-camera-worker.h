#pragma once
#include "mouse-control-worker.h"
#include "../force-controls.h"

namespace ModMenuModule {
	class RotateCameraWorker : public MouseControlWorker {
	public:
		RotateCameraWorker() = default;
		RotateCameraWorker(const RotateCameraWorker&) = delete;
		RotateCameraWorker& operator=(const RotateCameraWorker&) = delete;

		void Start() override;
		void Stop() override;
		bool IsRunning() const override { return m_running; }
		void Update() override;

		void SaveInitialCameraOffset();
		void RestoreInitialCameraOffset() const;

	private:
		bool m_running = false;
		float m_initialCameraOffset = 0.0f;
	};
}

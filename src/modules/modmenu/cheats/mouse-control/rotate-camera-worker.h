#pragma once
#include "mouse-control-worker.h"
#include "../camera/camera.h"

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

		bool RemoveOnGamePause() const override { return false; }

	private:
		bool EnsureFPROHandle();
		void FreeFPROHandle();

		bool m_running = false;
		CameraCheatFPROHandleId m_FPROHandleId = -1;
		float m_currentOffset = 0.0f;
	};
}

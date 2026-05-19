#pragma once
#include "mouse-control-worker.h"
#include "../force-controls.h"

namespace ModMenuModule {
	class PointAtMouseWorker : public MouseControlWorker, public Core::EventListenerSupport {
	public:
		PointAtMouseWorker() = default;
		PointAtMouseWorker(const PointAtMouseWorker&) = delete;
		PointAtMouseWorker& operator=(const PointAtMouseWorker&) = delete;

		void Start() override;
		void Stop() override;
		bool IsRunning() const override { return m_running; }
		void Update() override;

	private:
		void OnMouseMove(MouseModule::MouseMoveEvent& event);

		void UpdateLastMousePosition();

		static std::optional<float> GetTargetRotation(MouseModule::NormalizedMousePosition normalizedPos);
		static float GetPlayerPedRotation();

		bool m_running = false;
		MouseModule::NormalizedMousePosition m_lastMousePosition{ 0.0f, 0.0f };
	};
}

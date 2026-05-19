#pragma once

namespace ModMenuModule {
	class MouseControlCheat;

	class MouseControlWorker {
	public:
		virtual ~MouseControlWorker() = default;

		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual bool IsRunning() const = 0;
		virtual void Update() {}

	protected:
		MouseControlWorker() = default;
		MouseControlWorker(const MouseControlWorker&) = delete;
		MouseControlWorker& operator=(const MouseControlWorker&) = delete;
	};
}

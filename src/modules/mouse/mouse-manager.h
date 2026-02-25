#pragma once
#include "common.h"
#include "mouse-state.h"
#include "../../events/wnd-proc.h"
#include "../../events/draw-frame.h"
#include "mouse-position.h"
#include "normalized-mouse-position.h"

namespace MouseModule {
	class MouseManager : public Core::EventListenerSupport {
	public:
		static MouseManager* GetInstance();
		static bool IsSafeToInitEvents();
		static MouseState FetchMouseState();
		static NormalizedMousePosition ToNormalizedPosition(const MousePosition& pos);
		static MousePosition ToPixelPosition(const NormalizedMousePosition& normalizedPos);

		std::optional<MouseState> GetLastMouseState() const;

		void SetLocked(bool locked);
		bool IsLocked() const { return m_locked; }

		void SetInvisible(bool invisible);
		bool IsInvisible() { return m_invisible; }

		bool MarkEventInitialized();
		void MarkEventDeinitialized();

	private:
		friend class RootModule;
		MouseManager();
		~MouseManager();
		MouseManager(const MouseManager&) = delete;
		MouseManager& operator=(const MouseManager&) = delete;

		static void SetToCenter();
		static MousePosition GetClientAreaSize();

		void OnWndProcEvent(WndProcEvent& event);
		void OnPreDrawFrame(PreDrawFrameEvent& event);

		bool Attach();
		void Detach();
		void UpdateState();
		void SetupTracking();
		void OnMouseEnter();
		void ResetButtons();
		void UpdateWndProcEventListener();
		void UpdateCursorOwned();
		void UpdateLockedState();
		void UpdateInvisible();

		static MouseManager* m_instance;
		bool m_attached = false;
		bool m_tracking = false;
		bool m_invisible = false;
		bool m_locked = false;
		bool m_cursorOwned = false;
		bool m_lockedFirstTick = false;
		int m_initializedEvents = 0;
		MouseState m_state;
	};
}

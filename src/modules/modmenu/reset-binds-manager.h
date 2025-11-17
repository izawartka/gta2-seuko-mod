#pragma once
#include "common.h"
#include "menu-base.h"
#include "../../events/keyboard.h"
#include "../../events/draw.h"
#include "../../events/game-end.h"

namespace ModMenuModule {
	class ResetBindsManager : public Core::EventListenerSupport {
	public:
		static ResetBindsManager* GetInstance();

		void Reset();

	private:
		friend class RootModule;
		ResetBindsManager();
		virtual ~ResetBindsManager();
		ResetBindsManager(const ResetBindsManager&) = delete;
		ResetBindsManager& operator=(const ResetBindsManager&) = delete;

		void Attach();
		void Detach();

		static ResetBindsManager* m_instance;

		void OnKeyDown(KeyDownEvent& event);
		void OnKeyUp(KeyUpEvent& event);
		void OnPreDrawUI(PreDrawUIEvent& event);
		void OnPreGameEnd(PreGameEndEvent& event);

		unsigned int m_resetKeyHoldFrames = 0;
	};
}
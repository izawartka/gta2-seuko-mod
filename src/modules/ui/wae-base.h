#pragma once
#include "common.h"

namespace UiModule {
	class WaeBase {
	public:
		using WaeEditStopCallback = std::function<void()>;

		virtual void SetWatching(bool watching) {};
		virtual bool IsWatching() const { return m_watching; }

		virtual void SetActive(bool active) {};
		virtual bool IsActive() const { return m_active; }

		virtual void SetEditing(bool editing) {};
		virtual bool IsEditing() const { return m_editing; }

		virtual void SetEditStopCallback(WaeEditStopCallback callback) {
			m_onEditStop = callback;
		}

	protected:
		WaeBase() = default;
		virtual ~WaeBase() = default;

		bool m_watching = false;
		bool m_active = false;
		bool m_editing = false;

		WaeEditStopCallback m_onEditStop = nullptr;
	};
}

#pragma once
#include "../common.h"
#include "../controller.h"

namespace UiModule {
	class MenuItemController : public Controller {
	public:
		using MenuItemEditStopCallback = std::function<void()>;

		virtual void SetWatching(bool watching) {};
		virtual bool IsWatching() const { return m_watching; }

		virtual void SetActive(bool active) {};
		virtual bool IsActive() const { return m_active; }

		virtual void SetEditing(bool editing) {};
		virtual bool IsEditing() const { return m_editing; }

		virtual void SetEditStopCallback(MenuItemEditStopCallback callback) {
			m_onEditStop = callback;
		}

	protected:
		MenuItemController() = default;
		virtual ~MenuItemController() = default;

		bool m_watching = false;
		bool m_active = false;
		bool m_editing = false;

		MenuItemEditStopCallback m_onEditStop = nullptr;
	};
}

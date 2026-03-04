#pragma once
#include "../common.h"
#include "../controller.h"

namespace UiModule {
	class MenuController;

	class MenuItemController : public Controller {
	public:
		using MenuItemEditStateChangeCallback = std::function<void(bool)>;
		using MenuItemEditStopCallback = std::function<void()>;

		/*
		Set whether the controller is watching for changes in the variable it controls.
		*/
		virtual void SetWatching(bool watching) { }

		/*
		Get whether the controller is watching for changes in the variable it controls.
		*/
		virtual bool IsWatching() const { return m_watching; }

		/*
		Set whether the controller is active and should respond to user input.
		*/
		virtual void SetActive(bool active) { }

		/*
		Get whether the controller is active and should respond to user input.
		*/
		virtual bool IsActive() const { return m_active; }

		/*
		Set whether the controller is in editing mode, meaning that the user is currently editing the value it controls.
		Not all controllers implement editing mode.
		Setting editing to true, if succeeds, also temporarily sets the controller to active.
		Most controllers (if not in live mode) might also temporarily disable watching while editing to prevent changes from being applied while the user is editing.
		*/
		virtual void SetEditing(bool editing) { 
			if (m_onEditStop) m_onEditStop();
		};

		/*
		Get whether the controller is in editing mode, meaning that the user is currently editing the value it controls.
		*/
		virtual bool IsEditing() const { return m_editing; }

		/*
		Called after the edit state changes, with the new editing state as a parameter
		*/
		virtual void SetEditStateChangeCallback(MenuItemEditStateChangeCallback callback) {
			m_onEditStateChange = callback;
		}

		/* 
		Called when the item stops being or when the request to start editing is cancelled by the controller.
		Used mainly to give the control back to the menu controller that owns it
		*/
		virtual void SetEditStopCallback(MenuItemEditStopCallback callback) {
			m_onEditStop = callback;
		}

	protected:
		MenuItemController() = default;
		virtual ~MenuItemController() = default;

		bool m_watching = false;
		bool m_active = false;
		bool m_editing = false;

		MenuItemEditStateChangeCallback m_onEditStateChange = nullptr;
		MenuItemEditStopCallback m_onEditStop = nullptr;
	};
}

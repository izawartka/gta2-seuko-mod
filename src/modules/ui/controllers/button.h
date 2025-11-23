#pragma once
#include "../common.h"
#include "menu-item.h"
#include "../standard-binds-support.h"
#include "../components/text.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	using ButtonCallback = std::function<void()>;

	struct ButtonControllerOptions {
		StandardBindsSupportOptions keyBindOptions = {};
	};

	class ButtonController : public MenuItemController, public Core::EventListenerSupport, public StandardBindsSupport {
	public:
		ButtonController(Text* text, const ButtonControllerOptions& options = {})
			: StandardBindsSupport::StandardBindsSupport(options.keyBindOptions)
		{
			m_textComponent = text;
			m_options = options;
		}

		virtual ~ButtonController() {
			SetCallback(nullptr);
			SetEditStopCallback(nullptr);

			SetWatching(false);
			SetActive(false);
			SetEditing(false);
		}

		virtual void SetActive(bool active) override {
			if (active == m_active) return;
			m_active = active;

			if (active) {
				AddEventListener<KeyDownEvent>(&ButtonController::OnKeyDown);
			}
			else {
				RemoveEventListener<KeyDownEvent>();
			}
		}

		virtual void SetEditing(bool editing) override {
			if (!editing) return;
			if (m_onEditStop) {
				m_onEditStop();
			}
			Action();
		}

		void SetCallback(ButtonCallback callback) {
			m_callback = callback;
		}

		template <typename T, typename U>
		void SetCallback(U* instance, void (T::*method)()) {
			m_callback = [instance, method]() {
				(instance->*method)();
			};
		}

		void Action() {
			if (m_callback) {
				m_callback();
			}
		}

	protected:
		void OnKeyDown(KeyDownEvent& event) {
			if (!m_active) return;

			KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyDownEvent(event);
			if (IsActionKey(key)) {
				Action();
				return;
			}
		}

		ButtonControllerOptions m_options;
		Text* m_textComponent = nullptr;
		ButtonCallback m_callback = nullptr;
	};
}

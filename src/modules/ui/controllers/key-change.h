#pragma once
#include "../common.h"
#include "menu-item.h"
#include "../standard-binds-support.h"
#include "../components/text.h"
#include "../../../converters/key.h"
#include "../../../events/draw.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	using KeyChangeSaveCallback = std::function<void(KeyBindingModule::Key newValue)>;

	struct KeyChangeControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring marker = L" *";
		std::wstring waitingText = L"Press a key...";
		StandardBindsSupportOptions keyBindOptions = {};
		bool noShift = false;
		bool noCtrl = false;
		bool noAlt = false;
	};

	class KeyChangeController : public MenuItemController, public Core::EventListenerSupport, public StandardBindsSupport {
	public:
		KeyChangeController(Text* text, std::optional<KeyBindingModule::Key> value, KeyChangeControllerOptions options = {})
			: StandardBindsSupport::StandardBindsSupport(options.keyBindOptions)
		{
			m_textComponent = text;
			m_value = value;
			m_options = options;

			UpdateTextBuffer();
			UpdateText();
		}

		virtual ~KeyChangeController() {
			SetSaveCallback(nullptr);
			SetEditStopCallback(nullptr);

			SetWatching(false);
			SetActive(false);
			SetEditing(false);
		}

		virtual void SetActive(bool active) override {
			m_activeBeforeEdit = active;
			if (active == m_active) return;
			m_active = active;

			if (active) {
				AddEventListener<KeyDownEvent>(&KeyChangeController::OnKeyDown);
			}
			else {
				RemoveEventListener<KeyDownEvent>();
			}
		}

		virtual void SetEditing(bool editing) override {
			if (m_editing == editing) return;
			m_editing = editing;
			if (editing) {
				bool active = m_active;
				SetActive(true);
				m_activeBeforeEdit = active;
				UpdateTextBuffer();
				UpdateText();
			}
			else {
				if (!m_activeBeforeEdit) {
					SetActive(false);
				}
				UpdateTextBuffer();
				UpdateText();
				if (m_onEditStop) m_onEditStop();
			}
		}

		void SetSaveCallback(KeyChangeSaveCallback callback) {
			m_saveCallback = callback;
		}

		void SetValue(std::optional<KeyBindingModule::Key> value) {
			bool hasValue = value.has_value();
			m_value = value;

			if (m_editing) return;

			UpdateTextBuffer();
			UpdateText();
		}

		std::optional<KeyBindingModule::Key> GetValue() const {
			return m_value;
		}

	protected:
		void Save(KeyBindingModule::Key newValue) {
			if (!m_editing) return;
			m_value = newValue;

			if (m_saveCallback) {
				m_saveCallback(newValue);
			}
		}

		void UpdateTextBuffer() {
			if (m_editing) {
				m_textBuffer = m_options.waitingText;
			}
			else if (m_value.has_value()) {
				m_textBuffer = KeyConverter<true>::ConvertToString(m_value.value());
			}
			else {
				m_textBuffer = L"Unbound";
			}
		}

		void UpdateText() {
			std::wstring marker = m_editing ? m_options.marker : L"";
			m_textComponent->SetText(m_options.prefix + m_textBuffer + marker + m_options.suffix);
		}

		void OnKeyDown(KeyDownEvent& event) {
			if (!m_active) return;

			KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyDownEvent(event);

			if (!m_editing) {
				if (IsActionKey(key)) {
					SetEditing(true);
				}
				return;
			}

			Game::KeyCode keyCode = event.GetKeyCode();
			if (
				keyCode == Game::KeyCode::DIK_LSHIFT ||
				keyCode == Game::KeyCode::DIK_RSHIFT ||
				keyCode == Game::KeyCode::DIK_LCONTROL ||
				keyCode == Game::KeyCode::DIK_RCONTROL ||
				keyCode == Game::KeyCode::DIK_LMENU ||
				keyCode == Game::KeyCode::DIK_RMENU
				) {
				return; // ignore modifier-only presses
			}

			if (m_options.noShift) {
				spdlog::debug("Ignoring shift modifier due to controller options");
				key.shift = false;
			}
			if (m_options.noCtrl) {
				spdlog::debug("Ignoring ctrl modifier due to controller options");
				key.ctrl = false;
			}
			if (m_options.noAlt) {
				spdlog::debug("Ignoring alt modifier due to controller options");
				key.alt = false;
			}

			Save(key);
			SetEditing(false);
		}

		KeyChangeControllerOptions m_options;
		Text* m_textComponent = nullptr;
		std::optional<KeyBindingModule::Key> m_value = std::nullopt;
		bool m_activeBeforeEdit = false;
		std::wstring m_textBuffer = L"";
		KeyChangeSaveCallback m_saveCallback = nullptr;
	};
}

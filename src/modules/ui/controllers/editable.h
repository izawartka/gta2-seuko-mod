#pragma once
#include "../common.h"
#include "menu-item.h"
#include "../converter-support.h"
#include "../standard-binds-support.h"
#include "../components/text.h"
#include "../../../events/draw.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	template <typename T>
	using EditableSaveCallback = std::function<void(T newValue)>;

	struct EditableControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring nullText = L"N/A";
		std::wstring marker = L"-";
		StandardBindsSupportOptions keyBindOptions = {};
		int blinkInterval = 15; // frames
	};

	template <typename T>
	class EditableController : public MenuItemController, public Core::EventListenerSupport, public ConverterSupport<T>, public StandardBindsSupport {
	public:
		EditableController(Text* text, const std::optional<T>& value, const EditableControllerOptions& options = {})
			: StandardBindsSupport::StandardBindsSupport(options.keyBindOptions)
		{
			static_assert(std::is_copy_constructible<T>::value, "T must be copy-constructible");
			m_textComponent = text;
			m_options = options;
			m_value = value;
			m_displayValue = value;

			UpdateTextBuffer();
			UpdateText();
		}

		virtual ~EditableController() {
			SetEditStopCallback(nullptr);
			SetSaveCallback(nullptr);

			SetActive(false);
			SetEditing(false);
		}

		virtual void SetActive(bool active) override {
			m_activeBeforeEdit = active;
			if (active == m_active) return;
			m_active = active;

			if (active) {
				AddEventListener<KeyDownEvent>(&EditableController<T>::OnKeyDown);
				if (m_editing) SetPreDrawUIListener(true);
			}
			else {
				RemoveEventListener<KeyDownEvent>();
				SetPreDrawUIListener(false);
			}
		}

		virtual void SetEditing(bool editing) override {
			if (m_editing == editing) return;
			m_editing = editing;
			if (editing) {
				if (!m_value.has_value()) {
					spdlog::warn("Invalid value, cannot edit");
					m_editing = false;
					if (m_onEditStop) m_onEditStop();
					return;
				}
				bool active = m_active;
				SetActive(true);
				m_activeBeforeEdit = active;
				// prepare text buffer for editing from current value
				if (m_displayValue.has_value()) {
					m_textBuffer = this->ConvertToString(m_displayValue.value());
				}
				else {
					m_textBuffer = m_options.nullText;
				}
				UpdateText();
			}
			else {
				if (!m_activeBeforeEdit) {
					SetActive(false);
				}
				m_displayValue = m_value;
				UpdateTextBuffer();
				UpdateText();
				if (m_onEditStop) m_onEditStop();
			}
		}

		void SetSaveCallback(EditableSaveCallback<T> callback) {
			m_saveCallback = callback;
		}

		void SetValue(const std::optional<T>& value) {
			m_value = value;
			if (m_editing) {
				// do not update displayed/editing buffer while editing
				return;
			}
			m_displayValue = value;
			UpdateTextBuffer();
			UpdateText();
		}

		const std::optional<T>& GetValue() const {
			return m_value;
		}

	protected:
		virtual void OnConverterChanged() override {
			if (m_editing) return;

			UpdateTextBuffer();
			UpdateText();
		}

		void SetPreDrawUIListener(bool enable) {
			if (enable == m_hasPreDrawListener) return;
			m_hasPreDrawListener = enable;
			if (enable) {
				AddEventListener<PreDrawUIEvent>(&EditableController<T>::OnPreDrawUI);
				m_blinkCounter = 0;
			}
			else {
				RemoveEventListener<PreDrawUIEvent>();
			}
		}

		void Save() {
			if (!m_editing) return;
			T newValue;

			try {
				newValue = this->ConvertFromString(m_textBuffer);
			}
			catch (const std::exception& e) {
				spdlog::warn("Failed to parse input '{}': {}", std::string(m_textBuffer.begin(), m_textBuffer.end()), e.what());
				return;
			}

			m_value = newValue;
			m_displayValue = m_value;

			if (m_saveCallback) {
				m_saveCallback(newValue);
			}
			UpdateTextBuffer();
		}

		void UpdateTextBuffer() {
			if (m_displayValue.has_value()) {
				m_textBuffer = this->ConvertToString(m_displayValue.value());
			}
			else {
				m_textBuffer = m_options.nullText;
			}
		}

		void UpdateText() {
			std::wstring marker = m_editing && (m_blinkCounter < m_options.blinkInterval) ? m_options.marker : L"";
			m_textComponent->SetText(m_options.prefix + m_textBuffer + marker + m_options.suffix);
		}

		void OnKeyDown(KeyDownEvent& event) {
			if (!m_active) return;

			Game::KeyCode keyCode = event.GetKeyCode();
			bool isShiftPressed = event.IsShiftPressed();
			KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyDownEvent(event);

			if (!m_editing) {
				if (IsActionKey(key)) {
					SetEditing(true);
				}
				return;
			}

			if (IsActionKey(key)) {
				Save();
				SetEditing(!m_editing);
				return;
			}
			else if (keyCode == Game::KeyCode::DIK_BACK) {
				if (!m_textBuffer.empty()) {
					m_textBuffer.pop_back();
				}
				UpdateText();
			}
			else {
				char c = Game::GetCharFromKeyCode(keyCode, isShiftPressed, false); /// TODO handle caps lock
				if (c == '\0') return;

				if (!this->IsValidChar(m_textBuffer, static_cast<wchar_t>(c))) return;

				m_textBuffer += c;
				UpdateText();
			}
		}

		void OnPreDrawUI(PreDrawUIEvent& event) {
			if (!m_editing) return;
			if (m_blinkCounter == 0 || m_blinkCounter == m_options.blinkInterval) {
				UpdateText();
			}
			m_blinkCounter = (m_blinkCounter + 1) % (m_options.blinkInterval * 2);
		}

		EditableControllerOptions m_options;
		Text* m_textComponent = nullptr;
		std::optional<T> m_value = std::nullopt;
		std::optional<T> m_displayValue = std::nullopt;
		bool m_activeBeforeEdit = false;
		bool m_hasPreDrawListener = false;
		std::wstring m_textBuffer = L"";
		EditableSaveCallback<T> m_saveCallback = nullptr;
		int m_blinkCounter = 0;
	};
}

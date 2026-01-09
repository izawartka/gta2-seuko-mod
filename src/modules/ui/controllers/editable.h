#pragma once
#include "../common.h"
#include "menu-item.h"
#include "../converter-support.h"
#include "../standard-binds-support.h"
#include "../components/text.h"
#include "../events/update-ui.h"
#include "../events/key-down-repeat.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	template <typename T>
	using EditableSaveCallback = std::function<void(const T& newValue)>;

	template <typename T>
	using EditableValidateCallback = std::function<bool(const T& newValue)>;

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
				AddEventListener<KeyDownRepeatEvent>(&EditableController<T>::OnKeyDownRepeat);
			}
			else {
				RemoveEventListener<KeyDownEvent>();
				RemoveEventListener<KeyDownRepeatEvent>();
			}
		}

		virtual void SetEditing(bool editing) override {
			if (m_editing == editing) return;
			m_editing = editing;
			if (editing) {
				bool active = m_active;
				SetActive(true);
				m_activeBeforeEdit = active;
				SetUpdateUIListener(true);
				UpdateTextBuffer();
				UpdateText();
			}
			else {
				if (!m_activeBeforeEdit) {
					SetActive(false);
				}
				SetUpdateUIListener(false);
				UpdateTextBuffer();
				UpdateText();
				if (m_onEditStop) m_onEditStop();
			}
		}

		void SetSaveCallback(EditableSaveCallback<T> callback) {
			m_saveCallback = callback;
		}

		void SetValidateCallback(EditableValidateCallback<T> callback) {
			m_validateCallback = callback;
		}

		void SetValue(const std::optional<T>& value) {
			if(m_validateCallback && value.has_value() && !m_validateCallback(value.value())) {
				spdlog::warn("UiModule::EditableController: Attempted to set invalid value");
				return;
			}

			m_value = value;
			if (m_editing) {
				// do not update displayed/editing buffer while editing
				return;
			}
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

		void SetUpdateUIListener(bool enable) {
			if (enable == m_hasUpdateListener) return;
			m_hasUpdateListener = enable;
			if (enable) {
				AddEventListener<UiModule::UpdateUIEvent>(&EditableController<T>::OnUpdateUI);
				m_blinkCounter = 0;
			}
			else {
				RemoveEventListener<UiModule::UpdateUIEvent>();
			}
		}

		void Save() {
			if (!m_editing) return;
			T newValue;

			try {
				newValue = this->ConvertFromString(m_textBuffer);
			}
			catch (const std::exception& e) {
				spdlog::warn("UiModule::EditableController: Failed to parse input: {}", e.what());
				return;
			}

			if (m_validateCallback && !m_validateCallback(newValue)) {
				spdlog::warn("UiModule::EditableController: Input validation failed");
				return;
			}

			m_value = newValue;

			if (m_saveCallback) {
				m_saveCallback(newValue);
			}
		}

		void UpdateTextBuffer() {
			if (m_value.has_value()) {
				m_textBuffer = this->ConvertToString(m_value.value());
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

			KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyboardEvent(event);
			if (!IsActionKey(key)) return;

			if (!m_editing) {
				SetEditing(true);
				return;
			}

			Save();
			SetEditing(false);
		}

		void OnKeyDownRepeat(KeyDownRepeatEvent& event) {
			if (!m_active) return;

			Game::KeyCode keyCode = event.GetKeyCode();
			bool isShiftPressed = event.IsShiftPressed();

			if (keyCode == Game::KeyCode::DIK_BACK) {
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

		void OnUpdateUI(UiModule::UpdateUIEvent& event) {
			if (!m_editing) return;
			if (m_blinkCounter == 0 || m_blinkCounter == m_options.blinkInterval) {
				UpdateText();
			}
			m_blinkCounter = (m_blinkCounter + 1) % (m_options.blinkInterval * 2);
		}

		EditableControllerOptions m_options;
		Text* m_textComponent = nullptr;
		std::optional<T> m_value = std::nullopt;
		bool m_activeBeforeEdit = false;
		bool m_hasUpdateListener = false;
		std::wstring m_textBuffer = L"";
		EditableSaveCallback<T> m_saveCallback = nullptr;
		EditableValidateCallback<T> m_validateCallback = nullptr;
		int m_blinkCounter = 0;
	};
}

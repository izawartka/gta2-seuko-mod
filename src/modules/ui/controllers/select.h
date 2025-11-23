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
	using SelectSaveCallback = std::function<void(T newValue)>;

	template <typename T>
	using SelectOptionList = std::vector<T>;

	struct SelectControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring nullText = L"N/A";
		std::wstring marker = L" *";
		StandardBindsSupportOptions keyBindOptions = {};
		bool quickJump = true; // Allow jumping to the first / last value using shift + prev / next
		bool loop = false;
		bool liveMode = true; // immediately apply changes when selecting options and change selected option when value changes
	};

	template <typename T>
	class SelectController : public MenuItemController, public Core::EventListenerSupport, public ConverterSupport<T>, public StandardBindsSupport {
	public:
		SelectController(Text* text, const SelectOptionList<T>& optionList, const std::optional<T>& value, const SelectControllerOptions& options = {})
			: StandardBindsSupport::StandardBindsSupport(options.keyBindOptions) 
		{
			static_assert(std::is_copy_constructible<T>::value, "T must be copy-constructible");
			m_textComponent = text;
			m_options = options;
			m_optionList = optionList;

			SetValue(value);
		}

		virtual ~SelectController() {
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
				AddEventListener<KeyDownEvent>(&SelectController<T>::OnKeyDown);
			}
			else {
				RemoveEventListener<KeyDownEvent>();
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
				UpdateIndexFromValue();
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

		void SetSaveCallback(SelectSaveCallback<T> callback) {
			m_saveCallback = callback;
		}

		void SetValue(const std::optional<T>& value) {
			bool hasValue = value.has_value();
			m_value = value;

			if(m_editing && !m_options.liveMode) {
				return;
			}

			m_displayValue = value;
			UpdateTextBuffer();

			if (m_editing) {
				UpdateIndexFromValue();
			}

			UpdateText();
		}

		const std::optional<T>& GetValue() const {
			return m_value;
		}

	protected:
		virtual void OnConverterChanged() override {
			UpdateTextBuffer();
			UpdateText();
		}

		void Save() {
			if (!m_editing) return;
			if (m_currentIndex < 0 || m_currentIndex >= static_cast<int>(m_optionList.size())) {
				spdlog::warn("Invalid index, cannot save");
				return;
			}

			m_value = m_displayValue;

			if (m_saveCallback) {
				m_saveCallback(m_displayValue.value());
			}
		}

		void UpdateIndexFromValue() {
			m_currentIndex = -1;
			if (!m_displayValue.has_value()) {
				return;
			}
			auto it = std::find_if(m_optionList.begin(), m_optionList.end(), [this](const T& val) {
				return this->AreEqual(val, m_displayValue.value());
			});
			if (it == m_optionList.end()) {
				return;
			}
			m_currentIndex = std::distance(m_optionList.begin(), it);
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
			std::wstring marker = m_editing ? m_options.marker : L"";
			m_textComponent->SetText(m_options.prefix + m_textBuffer + marker + m_options.suffix);
		}

		void OnKeyDown(KeyDownEvent& event) {
			if (!m_active) return;

			bool isShiftPressed = event.IsShiftPressed();
			KeyBindingModule::Key key = KeyBindingModule::Key::FromKeyDownEvent(event);

			if (!m_editing) {
				if (IsActionKey(key)) {
					SetEditing(true);
				}
				return;
			}

			int newIndex = m_currentIndex;
			int optionCount = static_cast<int>(m_optionList.size());

			if (optionCount == 0) {
				return;
			}

			if (IsActionKey(key)) {
				Save();
				SetEditing(false);
				return;
			}
			else if (IsNextKey(key)) {
				newIndex++;
				if (isShiftPressed) newIndex = optionCount - 1;
				if (newIndex >= optionCount) {
					if (m_options.loop) {
						newIndex = 0;
					}
					else {
						newIndex = optionCount - 1;
					}
				}
			}
			else if (IsPrevKey(key)) {
				newIndex--;
				if (isShiftPressed) newIndex = 0;
				if (newIndex < 0) {
					if (m_options.loop) {
						newIndex = optionCount - 1;
					}
					else {
						newIndex = 0;
					}
				}
			}
			else {
				return;
			}

			if (newIndex == m_currentIndex) return;

			m_currentIndex = newIndex;
			m_displayValue = m_optionList[m_currentIndex];
			UpdateTextBuffer();
			if (m_options.liveMode) {
				Save();
			}
			UpdateText();
		}

		SelectControllerOptions m_options;
		Text* m_textComponent = nullptr;
		bool m_activeBeforeEdit = false;
		std::wstring m_textBuffer = L"";
		SelectSaveCallback<T> m_saveCallback = nullptr;
		SelectOptionList<T> m_optionList;
		std::optional<T> m_displayValue = std::nullopt;
		std::optional<T> m_value = std::nullopt;
		int m_currentIndex = 0;
	};
}

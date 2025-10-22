#pragma once
#include "../common.h"
#include "menu-item.h"
#include "../converter-support.h"
#include "../components/text.h"
#include "../../../events/draw.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	template <typename T>
	using VarTextEditableCustomSaveCallback = std::function<void(T newValue)>;

	struct VarTextEditableControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring nullText = L"N/A";
		std::wstring marker = L"-";
		Game::KeyCode keyAction = Game::KeyCode::DIK_BACKSLASH;
		int blinkInterval = 15; // frames
	};

	template <typename T>
	class VarTextEditableController : public MenuItemController, public Core::EventListenerSupport, public ConverterSupport<T> {
	public:
		VarTextEditableController(Text* text, Core::Resolver<T> resolver, VarTextEditableControllerOptions options = {}) {
			static_assert(std::is_copy_constructible<T>::value, "T must be copy-constructible");
			m_textComponent = text;
			m_options = options;
			m_resolver = resolver;

			m_textBuffer = m_options.nullText;
			UpdateText();
		}

		virtual ~VarTextEditableController() {
			SetEditStopCallback(nullptr);
			SetCustomSaveCallback(nullptr);

			SetWatching(false);
			SetActive(false);
			SetEditing(false);
		}

		virtual void SetWatching(bool watching) override {
			m_watchingBeforeEdit = watching;
			if (m_editing && watching) return;
			if (m_watching == watching) return;
			m_watching = watching;
			if (watching) {
				m_watched = Core::WatchManager::GetInstance()->Watch<PreDrawUIEvent, T>(
					m_resolver,
					this,
					&VarTextEditableController<T>::OnValueUpdate
				);
			}
			else {
				Core::WatchManager::GetInstance()->Unwatch(m_watched);
				m_watched = nullptr;
			}
		}

		virtual void SetActive(bool active) override {
			m_activeBeforeEdit = active;
			if (active == m_active) return;
			m_active = active;

			if (active) {
				AddEventListener<KeyDownEvent>(&VarTextEditableController<T>::OnKeyDown);
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
				if (m_pendingSaveValue.has_value()) {
					spdlog::warn("Save pending, cannot edit");
					m_editing = false;
					if (m_onEditStop) m_onEditStop();
					return;
				}
				bool watching = m_watching;
				bool active = m_active;
				SetWatching(false);
				SetActive(true);
				m_watchingBeforeEdit = watching;
				m_activeBeforeEdit = active;
				UpdateText();
			}
			else {
				if (m_watchingBeforeEdit) {
					SetWatching(true);
				}
				if (!m_activeBeforeEdit) {
					SetActive(false);
				}
				SetPreDrawUIListener(false);
				UpdateText();
				if (m_onEditStop) m_onEditStop();
			}
		}

		void SetCustomSaveCallback(VarTextEditableCustomSaveCallback<T> callback) {
			 m_customSaveCallback = callback;
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
				AddEventListener<PreDrawUIEvent>(&VarTextEditableController<T>::OnPreDrawUI);
				m_blinkCounter = 0;
			}
			else {
				RemoveEventListener<PreDrawUIEvent>();
			}
		}

		void Save() {
			if (!m_editing) return;
			if (!m_watchingBeforeEdit) return;
			T newValue;

			try {
				newValue = this->ConvertFromString(m_textBuffer);
			}
			catch (const std::exception& e) {
				spdlog::warn("Failed to parse input '{}': {}", std::string(m_textBuffer.begin(), m_textBuffer.end()), e.what());
				return;
			}

			m_pendingSaveValue = newValue;
		}

		void ApplyPendingSave() {
			T newValue = m_pendingSaveValue.value();
			m_pendingSaveValue = std::nullopt;

			if (m_customSaveCallback) {
				m_customSaveCallback(newValue);
			} else if (!m_watched->SetValue(newValue)) {
				spdlog::warn("Failed to resolve variable for setting new value");
			}

			m_watched->RequestUpdate();
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

		void OnValueUpdate(std::optional<T> oldValue, std::optional<T> newValue) {
			if (m_editing) {
				spdlog::warn("Tried to update VarTextEditableController value while editing");
				return;
			}

			if (m_pendingSaveValue.has_value()) {
				ApplyPendingSave();
				return;
			}

			m_value = newValue;

			UpdateTextBuffer();
			UpdateText();
		}

		void OnKeyDown(KeyDownEvent& event) {
			if (!m_active) return;

			Game::KeyCode key = event.GetKeyCode();
			bool isShiftPressed = event.IsShiftPressed();

			if (!m_editing) {
				if (key == m_options.keyAction) {
					SetEditing(true);
				}
				return;
			}

			if (key == m_options.keyAction) {
				Save();
				SetEditing(!m_editing);
				return;
			} else if (key == Game::KeyCode::DIK_BACK) {
				if (!m_textBuffer.empty()) {
					m_textBuffer.pop_back();
				}
				UpdateText();
			} else {
				char c = Game::GetCharFromKeyCode(key, isShiftPressed, false); /// TODO handle caps lock
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

		VarTextEditableControllerOptions m_options;
		Text* m_textComponent = nullptr;
		Core::Resolver<T> m_resolver = nullptr;
		Core::Watched<T>* m_watched = nullptr;
		std::optional<T> m_value = std::nullopt;
		bool m_watchingBeforeEdit = false;
		bool m_activeBeforeEdit = false;
		bool m_hasPreDrawListener = false;
		std::wstring m_textBuffer = L"";
		std::optional<T> m_pendingSaveValue = std::nullopt;
		VarTextEditableCustomSaveCallback<T> m_customSaveCallback = nullptr;
		int m_blinkCounter = 0;
	};
}

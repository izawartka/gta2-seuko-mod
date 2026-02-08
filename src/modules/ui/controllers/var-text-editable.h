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
	template <typename ValueT>
	using VarTextEditableCustomSaveCallback = std::function<void(const ValueT& newValue)>;

	template <typename ValueT>
	using VarTextEditableValidateCallback = std::function<bool(const ValueT& newValue)>;

	template <typename ValueT>
	using VarTextEditableClampCallback = std::function<ValueT(const ValueT& value)>;

	struct VarTextEditableControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring nullText = L"N/A";
		std::wstring marker = L"-";
		StandardBindsSupportOptions keyBindOptions = {};
		int blinkInterval = 15; // frames
	};

	template <typename ValueT, typename ResRetT = typename Core::DefaultResRetT<ValueT>::type>
	class VarTextEditableController : public MenuItemController, public Core::EventListenerSupport, public ConverterSupport<ValueT>, public StandardBindsSupport {
	public:
		VarTextEditableController(Text* text, Core::Resolver<ResRetT> resolver, const VarTextEditableControllerOptions& options = {})
			: StandardBindsSupport::StandardBindsSupport(options.keyBindOptions)
		{
			static_assert(std::is_copy_constructible<ValueT>::value, "ValueT must be copy-constructible");
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
				m_watched = Core::WatchManager::GetInstance()->Watch<UiModule::UpdateUIEvent, ValueT, ResRetT>(
					m_resolver,
					this,
					&VarTextEditableController<ValueT, ResRetT>::OnValueUpdate
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
				AddEventListener<KeyDownEvent>(&VarTextEditableController<ValueT, ResRetT>::OnKeyDown);
				AddEventListener<KeyDownRepeatEvent>(&VarTextEditableController<ValueT, ResRetT>::OnKeyDownRepeat);
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
				if (!m_watched || !m_watched->GetSavedValue().has_value()) {
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
				SetUpdateUIListener(true);
				UpdateText();
			}
			else {
				if (m_watchingBeforeEdit) {
					SetWatching(true);
				}
				if (!m_activeBeforeEdit) {
					SetActive(false);
				}
				SetUpdateUIListener(false);
				UpdateText();
				if (m_onEditStop) m_onEditStop();
			}
		}

		void SetCustomSaveCallback(VarTextEditableCustomSaveCallback<ValueT> callback) {
			m_customSaveCallback = callback;
		}

		void SetValidateCallback(VarTextEditableValidateCallback<ValueT> callback) {
			m_validateCallback = callback;
		}

		void SetClampCallback(VarTextEditableClampCallback<ValueT> callback) {
			m_clampCallback = callback;
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
				AddEventListener<UiModule::UpdateUIEvent>(&VarTextEditableController<ValueT, ResRetT>::OnUpdateUI);
				m_blinkCounter = 0;
			}
			else {
				RemoveEventListener<UiModule::UpdateUIEvent>();
			}
		}

		void Save() {
			if (!m_editing) return;
			if (!m_watchingBeforeEdit) return;
			ValueT newValue;

			try {
				newValue = this->ConvertFromString(m_textBuffer);
			}
			catch (const std::exception& e) {
				spdlog::warn("UiModule::VarTextEditableController: Failed to parse input: {}", e.what());
				return;
			}

			m_pendingSaveValue = newValue;
		}

		void ApplyPendingSave() {
			ValueT newValue = m_pendingSaveValue.value();
			ValueT clampedNewValue = m_clampCallback ? m_clampCallback(newValue) : newValue;
			m_pendingSaveValue = std::nullopt;

			if (m_validateCallback && !m_validateCallback(clampedNewValue)) {
				spdlog::warn("UiModule::VarTextEditableController: Input validation failed");
			}
			else if (m_customSaveCallback) {
				m_customSaveCallback(clampedNewValue);
			}
			else {
				if constexpr (Core::WatchedHasSetValue_v<Core::Watched<ValueT, ResRetT>>) {
					if (!m_watched->SetValueNow(clampedNewValue, true)) {
						spdlog::warn("Failed to set new value on watched variable");
					}
				}
				else {
					spdlog::error("No custom save callback set and watched variable does not support SetValue");
				}
			}

			m_watched->RequestUpdate();
		}

		void UpdateTextBuffer() {
			if (m_watched && m_watched->GetSavedValue().has_value()) {
				m_textBuffer = this->ConvertToString(m_watched->GetSavedValue().value());
			}
			else {
				m_textBuffer = m_options.nullText;
			}
		}

		void UpdateText() {
			std::wstring marker = m_editing && (m_blinkCounter < m_options.blinkInterval) ? m_options.marker : L"";
			m_textComponent->SetText(m_options.prefix + m_textBuffer + marker + m_options.suffix);
		}

		void OnValueUpdate(const std::optional<ValueT>& oldValue, const std::optional<ValueT>& newValue) {
			if (m_editing) {
				spdlog::warn("Tried to update VarTextEditableController value while editing");
				return;
			}

			if (m_pendingSaveValue.has_value()) {
				ApplyPendingSave();
				return;
			}

			UpdateTextBuffer();
			UpdateText();
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
			SetEditing(!m_editing);
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

		VarTextEditableControllerOptions m_options;
		Text* m_textComponent = nullptr;
		Core::Resolver<ResRetT> m_resolver = nullptr;
		Core::Watched<ValueT, ResRetT>* m_watched = nullptr;
		bool m_watchingBeforeEdit = false;
		bool m_activeBeforeEdit = false;
		bool m_hasUpdateListener = false;
		std::wstring m_textBuffer = L"";
		std::optional<ValueT> m_pendingSaveValue = std::nullopt;
		VarTextEditableCustomSaveCallback<ValueT> m_customSaveCallback = nullptr;
		VarTextEditableValidateCallback<ValueT> m_validateCallback = nullptr;
		VarTextEditableClampCallback<ValueT> m_clampCallback = nullptr;
		int m_blinkCounter = 0;
	};
}

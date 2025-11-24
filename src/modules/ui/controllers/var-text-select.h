#pragma once
#include "../common.h"
#include "menu-item.h"
#include "../converter-support.h"
#include "../standard-binds-support.h"
#include "../components/text.h"
#include "../events/update-ui.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	template <typename ValueT>
	using VarTextSelectCustomSaveCallback = std::function<void(const ValueT& newValue)>;

	template <typename ValueT>
	using VarTextSelectOptionList = std::vector<ValueT>;

	struct VarTextSelectControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring nullText = L"N/A";
		std::wstring marker = L" *";
		StandardBindsSupportOptions keyBindOptions = {};
		bool quickJump = true; // Allow jumping to the first / last value using shift + prev / next
		bool loop = false;
		bool liveMode = true; // immediately apply changes when selecting options and change selected option when variable changes
	};

	template <typename ValueT, typename ResRetT = typename Core::DefaultResRetT<ValueT>::type>
	class VarTextSelectController : public MenuItemController, public Core::EventListenerSupport, public ConverterSupport<ValueT>, public StandardBindsSupport {
	public:
		VarTextSelectController(Text* text, Core::Resolver<ResRetT> resolver, const VarTextSelectOptionList<ValueT>& optionList, const VarTextSelectControllerOptions& options = {})
			: StandardBindsSupport::StandardBindsSupport(options.keyBindOptions)
		{
			static_assert(std::is_copy_constructible<ValueT>::value, "ValueT must be copy-constructible");
			m_textComponent = text;
			m_options = options;
			m_resolver = resolver;
			m_optionList = optionList;

			m_textBuffer = m_options.nullText;
			UpdateText();
		}

		virtual ~VarTextSelectController() {
			SetCustomSaveCallback(nullptr);
			SetEditStopCallback(nullptr);

			SetWatching(false);
			SetActive(false);
			SetEditing(false);
		}

		virtual void SetWatching(bool watching) override {
			m_watchingBeforeEdit = watching;
			if (!m_options.liveMode && m_editing && watching) return;
			if (m_watching == watching) return;
			m_watching = watching;
			if (watching) {
				m_watched = Core::WatchManager::GetInstance()->Watch<UiModule::UpdateUIEvent, ValueT, ResRetT>(
					m_resolver,
					this,
					&VarTextSelectController<ValueT, ResRetT>::OnValueUpdate
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
				AddEventListener<KeyDownEvent>(&VarTextSelectController<ValueT, ResRetT>::OnKeyDown);
			}
			else {
				RemoveEventListener<KeyDownEvent>();
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
				bool active = m_active;
				bool watching = m_watching;
				SetActive(true);
				if (!m_options.liveMode) SetWatching(false);
				m_activeBeforeEdit = active;
				m_watchingBeforeEdit = watching;
				UpdateIndexFromValue();
				UpdateText();
			}
			else {
				if (!m_activeBeforeEdit) {
					SetActive(false);
				}
				if (m_watchingBeforeEdit) {
					SetWatching(true);
				}
				UpdateText();
				if (m_onEditStop) m_onEditStop();
			}
		}

		void SetCustomSaveCallback(VarTextSelectCustomSaveCallback<ValueT> callback) {
			m_customSaveCallback = callback;
		}

	protected:
		virtual void OnConverterChanged() override {
			UpdateTextBuffer();
			UpdateText();
		}

		void Save() {
			if (!m_editing) return;
			if (!m_watchingBeforeEdit) return;
			if (m_currentIndex < 0 || m_currentIndex >= static_cast<int>(m_optionList.size())) {
				spdlog::warn("Invalid index, cannot save");
				return;
			}

			m_pendingSaveValue = m_optionList[m_currentIndex];

			if (m_watched) {
				m_watched->RequestUpdate();
			}
		}

		void ApplyPendingSave() {
			ValueT newValue = m_pendingSaveValue.value();
			m_pendingSaveValue = std::nullopt;

			if (m_customSaveCallback) {
				m_customSaveCallback(newValue);
			}
			else {
				if constexpr (Core::WatchedHasSetValue_v<Core::Watched<ValueT, ResRetT>>) {
					if (!m_watched->SetValueNow(newValue, true)) {
						spdlog::warn("Failed to set new value on watched variable");
					}
				}
				else {
					spdlog::error("No custom save callback set and watched variable does not support SetValue");
				}
			}

			m_watched->RequestUpdate();
		}

		void UpdateIndexFromValue() {
			m_currentIndex = -1;
			if (!m_displayValue.has_value()) {
				return;
			}
			auto it = std::find_if(m_optionList.begin(), m_optionList.end(), [this](const ValueT& val) {
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

		void OnValueUpdate(const std::optional<ValueT>& oldValue, const std::optional<ValueT>& newValue) {
			if (!m_options.liveMode && m_editing) {
				spdlog::warn("Tried to update VarTextSelectController value while editing in non-live mode");
				return;
			}

			if (m_pendingSaveValue.has_value()) {
				ApplyPendingSave();
				return;
			}

			m_displayValue = newValue;

			UpdateTextBuffer();

			if (m_editing) {
				UpdateIndexFromValue();
			}
			UpdateText();
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
				if(!m_options.liveMode) Save();
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

		VarTextSelectControllerOptions m_options;
		Text* m_textComponent = nullptr;
		Core::Resolver<ResRetT> m_resolver = nullptr;
		Core::Watched<ValueT, ResRetT>* m_watched = nullptr;
		bool m_activeBeforeEdit = false;
		bool m_watchingBeforeEdit = false;
		std::wstring m_textBuffer = L"";
		std::optional<ValueT> m_pendingSaveValue = std::nullopt;
		VarTextSelectCustomSaveCallback<ValueT> m_customSaveCallback = nullptr;
		VarTextSelectOptionList<ValueT> m_optionList;
		std::optional<ValueT> m_displayValue = std::nullopt;
		int m_currentIndex = 0;
	};
}

#pragma once
#include "../common.h"
#include "../controller.h"
#include "../converter-support.h"
#include "../wae-base.h"
#include "../components/text.h"
#include "../../../events/draw.h"
#include "../../../events/keyboard.h"

namespace UiModule {
    template <typename T>
    using VarTextSelectCustomSaveCallback = std::function<void(T newValue)>;

    template <typename T>
    using VarTextSelectOptionList = std::vector<T>;

    struct VarTextSelectControllerOptions {
        std::wstring prefix = L"";
        std::wstring suffix = L"";
        std::wstring nullText = L"N/A";
        std::wstring marker = L" *";
        Game::KeyCode keyNext = Game::KeyCode::DIK_RBRACKET;
        Game::KeyCode keyPrev = Game::KeyCode::DIK_LBRACKET;
        Game::KeyCode keyAction = Game::KeyCode::DIK_BACKSLASH;
        bool quickJump = true; // Allow jumping to the first / last value using shift + prev / next
        bool loop = false;
        bool liveMode = true; // immediately apply changes when selecting options and change selected option when variable changes
    };

    template <typename T>
    class VarTextSelectController : public Controller, public Core::EventListenerSupport, public ConverterSupport<T>, public WaeBase {
    public:
        VarTextSelectController(Text* text, Core::Resolver<T> resolver, VarTextSelectOptionList<T> optionList, VarTextSelectControllerOptions options = {}) {
            static_assert(std::is_copy_constructible<T>::value, "T must be copy-constructible");
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
                m_watched = Core::WatchManager::GetInstance()->Watch<PreDrawUIEvent, T>(
                    m_resolver,
                    this,
                    &VarTextSelectController<T>::OnValueUpdate
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
                AddEventListener<KeyDownEvent>(&VarTextSelectController<T>::OnKeyDown);
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
                bool watching = m_watching;
                SetActive(true);
                if(!m_options.liveMode) SetWatching(false);
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

        void SetCustomSaveCallback(VarTextSelectCustomSaveCallback<T> callback) {
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

            T newValue = m_optionList[m_currentIndex];
            m_pendingSaveValue = m_optionList[m_currentIndex];

            if (m_watched) {
                m_watched->RequestUpdate();
            }
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

        void OnValueUpdate(std::optional<T> oldValue, std::optional<T> newValue) {
            if (!m_options.liveMode && m_editing) {
                spdlog::warn("Tried to update VarTextSelectController value while editing in non-live mode");
                return;
            }

            if (m_pendingSaveValue.has_value()) {
                ApplyPendingSave();
                return;
            }

            m_value = newValue;
            m_displayValue = newValue;

            UpdateTextBuffer();

            if (m_editing) {
                UpdateIndexFromValue();
            }
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

            int newIndex = m_currentIndex;
            int optionCount = static_cast<int>(m_optionList.size());

            if (optionCount == 0) {
                return;
            }

            if (key == m_options.keyAction) {
                Save();
                SetEditing(false);
                return;
            }
            else if (key == m_options.keyNext) {
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
            else if (key == m_options.keyPrev) {
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
        Core::Resolver<T> m_resolver = nullptr;
        Core::Watched<T>* m_watched = nullptr;
        std::optional<T> m_value = std::nullopt;
        bool m_activeBeforeEdit = false;
        bool m_watchingBeforeEdit = false;
        std::wstring m_textBuffer = L"";
        std::optional<T> m_pendingSaveValue = std::nullopt;
        VarTextSelectCustomSaveCallback<T> m_customSaveCallback = nullptr;
        VarTextSelectOptionList<T> m_optionList;
        std::optional<T> m_displayValue = std::nullopt;
        int m_currentIndex = 0;
    };
}

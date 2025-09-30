#pragma once
#include "../common.h"
#include "../controller.h"
#include "../components/text.h"
#include "../utils/string-utils.h"
#include "../../../events/draw.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	using VarTextEditableEditStopCallback = std::function<void()>;

	struct VarTextEditableControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring nullText = L"N/A";
		std::wstring marker = L"-";
		Game::KeyCode keyAction = Game::KeyCode::DIK_BACKSLASH;
		bool allowControlChars = false; // whether to allow \t, \n, \r
		int blinkInterval = 15; // frames
		bool removeTrailingZeros = true; // for floating point types
	};

	template <typename T>
	class VarTextEditableController : public Controller, public Core::EventListenerSupport {
	public:
		VarTextEditableController(Text* text, Core::Resolver<T> resolver, VarTextEditableControllerOptions options = {}) {
			static_assert(std::is_copy_constructible<T>::value, "T must be copy-constructible");
			m_textComponent = text;
			m_options = options;
			m_resolver = resolver;

			Update();
		}

		virtual ~VarTextEditableController() {
			SetWatching(false);
		}

		void SetWatching(bool watching) {
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

			if(!m_editing) Update();
		}

		bool IsWatching() const { return m_watching; }

		void SetActive(bool active) {
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

		void SetEditing(bool editing) {
			if (m_editing == editing) return;
			m_editing = editing;
			if (editing) {
				if (!Load()) {
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
			}
			else {
				if (m_watchingBeforeEdit) {
					SetWatching(true);
				}
				if (!m_activeBeforeEdit) {
					SetActive(false);
				}
				SetPreDrawUIListener(false);
				m_editBuffer = L"";
				Update();
				
				if (m_onEditStop) m_onEditStop();
			}
		}

		void SetOnEditStopCallback(VarTextEditableEditStopCallback callback) {
			m_onEditStop = callback;
		}

		bool IsEditing() const { return m_editing; }

		void Update() {
			T* resolved = m_resolver();
			std::optional<T> value = resolved ? std::optional<T>(*resolved) : std::nullopt;
			OnValueUpdate(std::nullopt, value);
		}

	protected:
		bool Load() {
			if (!m_editing) return false;
			T* resolved = m_resolver();
			if (resolved == nullptr) {
				spdlog::warn("Failed to resolve variable for editing");
				return false;
			}

			m_editBuffer = ToString<T>(*resolved);
			if ((std::is_same_v<T, Game::SCR_f> || std::is_floating_point_v<T>) && m_options.removeTrailingZeros) {
				RemoveTrailingZeros(m_editBuffer);
			}
			return true;
		}

		void Save() {
			if (!m_editing) return;

			try {
				T newValue = FromString<T>(m_editBuffer);
				T* resolved = m_resolver();
				if (resolved) {
					*resolved = newValue;
				}
				else {
					spdlog::warn("Failed to resolve variable for setting new value");
				}
			}
			catch (const std::exception& e) {
				spdlog::warn("Failed to parse input '{}': {}", std::string(m_editBuffer.begin(), m_editBuffer.end()), e.what());
			}
		}

		void SetValueText(std::wstring text) {
			std::wstring marker = m_editing && (m_blinkCounter < m_options.blinkInterval) ? m_options.marker : L"";
			m_textComponent->SetText(m_options.prefix + text + marker + m_options.suffix);
		}

		void OnValueUpdate(std::optional<T> oldValue, std::optional<T> newValue) {
			if (m_editing) {
				spdlog::warn("Tried to update VarTextEditableController value while editing");
				return;
			}

			if (newValue.has_value()) {
				std::wstring text = ToString<T>(newValue.value());
				if ((std::is_same_v<T, Game::SCR_f> || std::is_floating_point_v<T>) && m_options.removeTrailingZeros) {
					UiModule::RemoveTrailingZeros(text);
				}
				SetValueText(text);
			}
			else {
				SetValueText(m_options.nullText);
			}
		}

		void OnKeyDown(const KeyDownEvent& event) {
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
				if (!m_editBuffer.empty()) {
					m_editBuffer.pop_back();
				}
				SetValueText(m_editBuffer);
			} else {
				char c = Game::GetCharFromKeyCode(key, isShiftPressed, false); /// TODO handle caps lock
				if (c == '\0') return;

				if (!IsValidCharForType<T>(c, m_options.allowControlChars)) return;

				m_editBuffer += c;
				SetValueText(m_editBuffer);
			}
		}

		void OnPreDrawUI(const PreDrawUIEvent& event) {
			if (!m_editing) return;
			if (m_blinkCounter == 0 || m_blinkCounter == m_options.blinkInterval) {
				SetValueText(m_editBuffer);
			}
			m_blinkCounter = (m_blinkCounter + 1) % (m_options.blinkInterval * 2);
		}

		VarTextEditableControllerOptions m_options;
		Text* m_textComponent = nullptr;
		Core::Resolver<T> m_resolver = nullptr;
		Core::Watched<T>* m_watched = nullptr;
		bool m_watching = false;
		bool m_editing = false;
		bool m_active = false;
		bool m_watchingBeforeEdit = false;
		bool m_activeBeforeEdit = false;
		bool m_hasPreDrawListener = false;
		std::wstring m_editBuffer = L"";
		VarTextEditableEditStopCallback m_onEditStop = nullptr;
		int m_blinkCounter = 0;
	};
}

#pragma once
#include "../common.h"
#include "menu-item.h"
#include "../standard-binds-support.h"
#include "../components/text.h"
#include "../../../converters/key.h"
#include "../../../events/draw.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	struct KeyBindChangeControllerOptions {
		std::wstring prefix = L"";
		std::wstring suffix = L"";
		std::wstring marker = L" *";
		std::wstring waitingText = L"Press a key...";
		StandardBindsSupportOptions keyBindOptions = {};
		bool noShift = false;
		bool noCtrl = false;
		bool noAlt = false;
	};

	class KeyBindChangeController : public MenuItemController, public Core::EventListenerSupport, public StandardBindsSupport {
	public:
		KeyBindChangeController(Text* text, std::string keyBindName, KeyBindingModule::Key keyBindDefault, KeyBindChangeControllerOptions options = {})
			: StandardBindsSupport::StandardBindsSupport(options.keyBindOptions)
		{
			m_textComponent = text;
			m_keyBindName = keyBindName;
			m_value = keyBindDefault;
			m_options = options;

			m_resolver = [this]() -> const KeyBindingModule::Key* {
				if (!m_value.has_value()) return nullptr;
				KeyBindingModule::BindManager* bindManager = KeyBindingModule::BindManager::GetInstance();
				return bindManager->GetOrCreateBind(m_keyBindName, m_value.value());
			};

			UpdateTextBuffer();
			UpdateText();
		}

		virtual ~KeyBindChangeController() {
			SetEditStopCallback(nullptr);

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
				m_watched = Core::WatchManager::GetInstance()->Watch<PreDrawUIEvent>(
					m_resolver,
					this,
					&KeyBindChangeController::OnValueUpdate
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
				AddEventListener<KeyDownEvent>(&KeyBindChangeController::OnKeyDown);
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
				SetWatching(false);
				m_activeBeforeEdit = active;
				m_watchingBeforeEdit = watching;
				UpdateTextBuffer();
				UpdateText();
			}
			else {
				if (!m_activeBeforeEdit) {
					SetActive(false);
				}
				if (m_watchingBeforeEdit) {
					SetWatching(true);
				}
				UpdateTextBuffer();
				UpdateText();
				if (m_onEditStop) m_onEditStop();
			}
		}

	protected:
		void Save() {
			if (!m_editing) return;
			if (!m_watchingBeforeEdit) return;

			KeyBindingModule::BindManager* bindManager = KeyBindingModule::BindManager::GetInstance();
			bindManager->SetBind(m_keyBindName, m_value.value());
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

		void OnValueUpdate(std::optional<KeyBindingModule::Key> oldValue, std::optional<KeyBindingModule::Key> newValue) {
			if (m_editing) {
				spdlog::warn("Tried to update KeyBindChangeController value while editing");
				return;
			}

			if (m_value == newValue) {
				return;
			}

			m_value = newValue;
			UpdateTextBuffer();
			UpdateText();
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

			m_value = key;
			Save();
			SetEditing(false);
		}

		KeyBindChangeControllerOptions m_options;
		std::string m_keyBindName;
		Text* m_textComponent = nullptr;
		Core::Resolver<const KeyBindingModule::Key*> m_resolver = nullptr;
		Core::Watched<KeyBindingModule::Key, const KeyBindingModule::Key*>* m_watched = nullptr;
		std::optional<KeyBindingModule::Key> m_value = std::nullopt;
		bool m_activeBeforeEdit = false;
		bool m_watchingBeforeEdit = false;
		std::wstring m_textBuffer = L"";
	};
}

#pragma once
#include "common.h"

namespace UiModule {
	struct StandardBindsSupportOptions {
		std::string keyBindNextName = "UI_Next";
		KeyBindingModule::Key keyBindNextDefault = KeyBindingModule::Key(Game::KeyCode::DIK_RBRACKET);
		std::string keyBindPrevName = "UI_Prev";
		KeyBindingModule::Key keyBindPrevDefault = KeyBindingModule::Key(Game::KeyCode::DIK_LBRACKET);
		std::string keyBindActionName = "UI_Action";
		KeyBindingModule::Key keyBindActionDefault = KeyBindingModule::Key(Game::KeyCode::DIK_BACKSLASH);
	};

	class StandardBindsSupport {
	public:
		const KeyBindingModule::Key* GetBindNextKey() const { return m_keyBindNext; }
		const KeyBindingModule::Key* GetBindPrevKey() const { return m_keyBindPrev; }
		const KeyBindingModule::Key* GetBindActionKey() const { return m_keyBindAction; }

		const bool IsNextKey(const KeyBindingModule::Key& key) const {
			return m_keyBindNext && KeyBindingModule::Key::ToNoShift(key) == *m_keyBindNext;
		}

		const bool IsPrevKey(const KeyBindingModule::Key& key) const {
			return m_keyBindPrev && KeyBindingModule::Key::ToNoShift(key) == *m_keyBindPrev;
		}

		const bool IsActionKey(const KeyBindingModule::Key& key) const {
			return m_keyBindAction && KeyBindingModule::Key::ToNoShift(key) == *m_keyBindAction;
		}

	protected:
		StandardBindsSupport(StandardBindsSupportOptions options = {}) {
			KeyBindingModule::BindManager* bindManager = KeyBindingModule::BindManager::GetInstance();
			m_keyBindPrev = bindManager->GetOrCreateBind(options.keyBindPrevName, KeyBindingModule::Key::ToNoShift(options.keyBindPrevDefault));
			m_keyBindNext = bindManager->GetOrCreateBind(options.keyBindNextName, KeyBindingModule::Key::ToNoShift(options.keyBindNextDefault));
			m_keyBindAction = bindManager->GetOrCreateBind(options.keyBindActionName, KeyBindingModule::Key::ToNoShift(options.keyBindActionDefault));
		}
		virtual ~StandardBindsSupport() = default;

	private:
		const KeyBindingModule::Key* m_keyBindNext = nullptr;
		const KeyBindingModule::Key* m_keyBindPrev = nullptr;
		const KeyBindingModule::Key* m_keyBindAction = nullptr;
	};
}
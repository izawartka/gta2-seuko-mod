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
		KeyBindingModule::KeyPtr GetBindNextKey() const { return m_keyBindNext; }
		KeyBindingModule::KeyPtr GetBindPrevKey() const { return m_keyBindPrev; }
		KeyBindingModule::KeyPtr GetBindActionKey() const { return m_keyBindAction; }

		const bool IsNextKey(const KeyBindingModule::Key& key) const {
			return !m_keyBindNext.expired() && KeyBindingModule::Key::ToNoShift(key) == *m_keyBindNext.lock();
		}

		const bool IsPrevKey(const KeyBindingModule::Key& key) const {
			return !m_keyBindPrev.expired() && KeyBindingModule::Key::ToNoShift(key) == *m_keyBindPrev.lock();
		}

		const bool IsActionKey(const KeyBindingModule::Key& key) const {
			return !m_keyBindAction.expired() && KeyBindingModule::Key::ToNoShift(key) == *m_keyBindAction.lock();
		}

	protected:
		StandardBindsSupport(const StandardBindsSupportOptions& options = {}) {
			KeyBindingModule::BindManager* bindManager = KeyBindingModule::BindManager::GetInstance();
			m_keyBindPrev = bindManager->GetOrCreateBind(options.keyBindPrevName, KeyBindingModule::Key::ToNoShift(options.keyBindPrevDefault));
			m_keyBindNext = bindManager->GetOrCreateBind(options.keyBindNextName, KeyBindingModule::Key::ToNoShift(options.keyBindNextDefault));
			m_keyBindAction = bindManager->GetOrCreateBind(options.keyBindActionName, KeyBindingModule::Key::ToNoShift(options.keyBindActionDefault));
		}
		virtual ~StandardBindsSupport() = default;

	private:
		KeyBindingModule::KeyPtr m_keyBindNext = {};
		KeyBindingModule::KeyPtr m_keyBindPrev = {};
		KeyBindingModule::KeyPtr m_keyBindAction = {};
	};
}
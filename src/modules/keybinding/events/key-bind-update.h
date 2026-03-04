#pragma once
#include "../common.h"

namespace KeyBindingModule {
	/*
	Dispatched after a key bind is added, updated or removed 
	Not dispatched after binds are loaded from persistence on module attach
	*/
	class KeyBindUpdateEvent : public Core::EventBase {
	public:
		KeyBindUpdateEvent(const std::string keyBindName) : m_keyBindName(keyBindName) {};
		virtual ~KeyBindUpdateEvent() override {};

		const std::string& GetKeyBindName() const { return m_keyBindName; }

	private:
		std::string m_keyBindName;
	};
}

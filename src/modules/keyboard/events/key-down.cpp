#include "key-down.h"
#include "../root.h"

bool KeyboardModule::KeyDownEvent::Init() {
	return KeyboardModule::RootModule::GetInstance() != nullptr;
}

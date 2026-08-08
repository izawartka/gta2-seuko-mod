#include "key-up.h"
#include "../root.h"

bool KeyboardModule::KeyUpEvent::Init() {
	return KeyboardModule::RootModule::GetInstance() != nullptr;
}

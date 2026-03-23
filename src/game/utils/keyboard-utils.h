#pragma once
#include "../gta2_wrapper.h"
#include "../constants.h"
#include "../keycode.h"

namespace Game::Utils {
	static constexpr KEYBOARD_STATE ToControlsStateOnly(KEYBOARD_STATE keyboardState) {
		return static_cast<KEYBOARD_STATE>(keyboardState & ((1U << Constants::CONTROLS_COUNT) - 1));
	}

	static constexpr size_t GetControlIndex(KEYBOARD_STATE control) {
		size_t shiftedControl = static_cast<size_t>(control) & ((1U << Constants::CONTROLS_COUNT) - 1);
		if (shiftedControl == 0) return -1;

		for (size_t i = 0; i < Constants::CONTROLS_COUNT; i++) {
			if (shiftedControl == 1) return i;
			shiftedControl = shiftedControl >> 1;
		}
		return -1;
	}

	static constexpr KEYBOARD_STATE GetControlBit(size_t controlIndex) {
		assert(controlIndex < Constants::CONTROLS_COUNT);

		return static_cast<KEYBOARD_STATE>(1U << controlIndex);
	}

	static constexpr bool IsControlDown(KEYBOARD_STATE keyboardState, size_t controlIndex) {
		assert(controlIndex < Constants::CONTROLS_COUNT);

		return (keyboardState & GetControlBit(controlIndex)) != 0;
	}

	static constexpr bool IsGamepadKey(KeyCode key) {
		uint32_t keyValue = static_cast<uint32_t>(key);
		return (keyValue >= 0xE0 && keyValue <= 0xE3) || keyValue == 0x00 || keyValue == 0x01;
	}
}

#pragma once
#include "common.h"
#include "keycode.h"

/*
Additional structs that use Game::KeyCode and cannot be placed in gta2.h
*/

namespace Game {
	struct Controls {
		KeyCode forward; // =0xE2 if controller
		KeyCode backward; // =0xE3 if controller
		KeyCode left; // =0xE0 if controller
		KeyCode right; // =0xE1 if controller
		KeyCode attack; // =0x00 if controller
		KeyCode enterCar; // =0x01 if controller
		KeyCode jump; // reused as handbrake
		KeyCode prevWeapon;
		KeyCode nextWeapon;
		KeyCode special; // burp
		KeyCode special2; // move camera
		KeyCode special3; // unused? right shift by default
	};
}

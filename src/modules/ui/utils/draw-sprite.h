#pragma once
#include "../common.h"

namespace UiModule::Utils {
	void DrawRotatedSprite(
		Game::SPRITE_TYPE type,
		uint16_t id,
		Game::PALETTE_BASE palette,
		uint16_t remap,
		float centerX,
		float centerY,
		int spriteWidth,
		int spriteHeight,
		float scale,
		float rotation,
		uint8_t alpha = 0xFF
	);

	void DrawSprite(
		Game::SPRITE_TYPE type,
		uint16_t id,
		Game::PALETTE_BASE palette,
		uint16_t remap,
		float centerX,
		float centerY,
		float spriteWidth,
		float spriteHeight,
		float scale,
		uint8_t alpha = 0xFF
	);

	void DrawSprite(
		Game::SPRITE_TYPE type,
		uint16_t id,
		Game::PALETTE_BASE palette,
		uint16_t remap,
		float x1,
		float y1,
		float x2,
		float y2,
		int spriteWidth,
		int spriteHeight,
		uint8_t alpha = 0xFF
	);
}

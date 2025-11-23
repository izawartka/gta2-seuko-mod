#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	struct SpriteOptions {
		Game::SPRITE_TYPE spriteType = Game::SPRITE_TYPE::SPRITE_TYPE_PED;
		uint16_t spriteId = 0;
		Game::PALETTE_BASE palette = Game::PALETTE_BASE::PALETTE_BASE_SPRITE;
		uint16_t remap = 0;
		Game::SCR_f scale = Game::Utils::FromFloat(1.0f);
		float rotation = 0.0f; // in radians
		uint8_t alpha = 255;
	};

	class Sprite : public Component {
	public:
		Sprite(Component* parent, const SpriteOptions& options);
		virtual void Draw() override;

		void SetOptions(const SpriteOptions& options);
		const SpriteOptions& GetOptions() const { return m_options; }

		int GetSpriteWidth() const { return m_spriteWidth; }
		int GetSpriteHeight() const { return m_spriteHeight; }

	protected:
		void RecalculateSize();

		SpriteOptions m_options;
		int m_spriteWidth = 0;
		int m_spriteHeight = 0;
	};
}
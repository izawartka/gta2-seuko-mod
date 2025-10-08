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
	};

	class Sprite : public Component {
	public:
		Sprite(Component* parent, SpriteOptions options);
		virtual void Draw() override;

		void SetOptions(SpriteOptions options);
		SpriteOptions GetOptions() const { return m_options; }

		int GetSpriteWidth() const { return m_spriteWidth; }
		int GetSpriteHeight() const { return m_spriteHeight; }

	protected:
		void RecalculateSize();

		SpriteOptions m_options;
		Game::GTAVertex m_vertices[4];

		int m_spriteWidth = 0;
		int m_spriteHeight = 0;
	};
}
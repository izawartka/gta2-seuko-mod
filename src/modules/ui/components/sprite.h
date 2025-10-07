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

		void SetSprite(Game::SPRITE_TYPE spriteType, uint16_t spriteId);
		Game::SPRITE_TYPE GetSpriteType() const { return m_spriteType; }
		uint16_t GetSpriteId() const { return m_spriteId; }

		void SetPalette(Game::PALETTE_BASE palette);
		Game::PALETTE_BASE GetPalette() const { return m_palette; }

		void SetRemap(uint16_t remap);
		uint16_t GetRemap() const { return m_remap; }

		void SetScale(Game::SCR_f scale);
		Game::SCR_f GetScale() const { return m_scale; }

		void SetRotation(float rotation); // in radians
		float GetRotation() const { return m_rotation; } // in radians

	protected:
		void RecalculateSize();

		Game::SPRITE_TYPE m_spriteType;
		uint16_t m_spriteId;
		Game::PALETTE_BASE m_palette;
		uint16_t m_remap;
		Game::GTAVertex m_vertices[4];
		Game::SCR_f m_scale;
		float m_rotation;

		int m_spriteWidth = 0;
		int m_spriteHeight = 0;
	};
}
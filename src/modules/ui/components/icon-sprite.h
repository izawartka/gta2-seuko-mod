#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	struct IconSpriteOptions {
		Game::SPRITE_TYPE spriteType = Game::SPRITE_TYPE::SPRITE_TYPE_PED;
		uint16_t spriteId = 0;
		Game::PALETTE_BASE palette = Game::PALETTE_BASE::PALETTE_BASE_SPRITE;
		uint16_t remap = 0;
		uint8_t alpha = 255;
		Game::SCR_f width = Game::Utils::FromFloat(20.0f);
		Game::SCR_f height = Game::Utils::FromFloat(20.0f);
		bool keepAspectRatio = true;
	};

	class IconSprite : public Component {
	public:
		IconSprite(Component* parent, const IconSpriteOptions& options);
		virtual void Draw() override;

		void SetOptions(const IconSpriteOptions& options);
		const IconSpriteOptions& GetOptions() const { return m_options; }

		int GetSpriteWidth() const { return m_spriteWidth; }
		int GetSpriteHeight() const { return m_spriteHeight; }

	protected:
		void RecalculateSize();

		IconSpriteOptions m_options;
		int m_spriteWidth = 0;
		int m_spriteHeight = 0;
	};
}

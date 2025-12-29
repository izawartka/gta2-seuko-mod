#pragma once
#include "../common.h"
#include "../component.h"

namespace UiModule {
	struct CarSpriteOptions {
		Game::CAR_MODEL4 carModel = Game::CAR_MODEL4_ALFA;
		Game::PALETTE_BASE palette = Game::PALETTE_BASE::PALETTE_BASE_SPRITE;
		uint16_t remap = 0;
		Game::SCR_f scale = Game::Utils::FromFloat(1.0f);
		float rotation = 0.0f; // in radians
		uint8_t alpha = 255;
		bool autoRoof = true;
		bool fixHotdogD3 = true;
	};

	class CarSprite : public Component {
	public:
		CarSprite(Component* parent, const CarSpriteOptions& options);
		virtual void Draw() override;

		void SetOptions(const CarSpriteOptions& options);
		const CarSpriteOptions& GetOptions() const { return m_options; }

		int GetSpriteWidth() const { return m_spriteWidth; }
		int GetSpriteHeight() const { return m_spriteHeight; }

	protected:
		void UpdateCarModelData();
		void UpdateRoofSpriteData();
		void RecalculateSize();

		struct RoofSprite {
			uint16_t spriteId = 0;
			int width = 0;
			int height = 0;
			int offsetX = 0;
			int offsetY = 0;
			Game::SCR_f centerX = 0;
			Game::SCR_f centerY = 0;
			bool isReversed = false;
		};

		CarSpriteOptions m_options;
		uint16_t m_spriteId = 0;
		int m_spriteWidth = 0;
		int m_spriteHeight = 0;
		Game::SCR_f m_spriteCenterX = 0;
		Game::SCR_f m_spriteCenterY = 0;
		RoofSprite m_roofSprite = {};
		int m_carWidth = 0;
		int m_carHeight = 0;
	};
}
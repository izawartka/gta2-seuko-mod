#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	static constexpr Game::SCR_f SPRITE_PREVIEW_SCALE = Game::Utils::FromFloat(2.0f);
	static constexpr Game::SCR_f SPRITE_PREVIEW_MARGIN_X = Game::Utils::FromFloat(24.0f);
	static constexpr Game::SCR_f SPRITE_PREVIEW_MARGIN_Y = Game::Utils::FromFloat(24.0f);

	class SpawnVehicleMenu : public MenuBase {
	public:
		SpawnVehicleMenu();
		virtual ~SpawnVehicleMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void UpdateSpritePreview();
		void UpdateSpritePreviewMargin();
		void Spawn();

		UiModule::SelectController<Game::CAR_MODEL4>* m_modelController = nullptr;
		UiModule::SelectController<std::tuple<short, Game::PALETTE_BASE>>* m_remapController = nullptr;

		UiModule::Margin* m_spritePreviewMargin = nullptr;
		UiModule::Sprite* m_spritePreview = nullptr;
		UiModule::SpriteOptions m_spritePreviewOptions;
	};
}

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

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void UpdateSpritePreview();
		void UpdateSpritePreviewMargin();
		void Spawn();

		UiModule::VarTextSelectController<Game::CAR_MODEL4>* m_modelController = nullptr;
		Game::CAR_MODEL4 m_selectedModel = Game::CAR_MODEL4::CAR_MODEL4_ALFA;
		UiModule::VarTextSelectController<bool>* m_unsupportedRemapsController = nullptr;
		bool m_unsupportedRemaps = false;
		UiModule::VarTextSelectController<short>* m_remapController = nullptr;
		short m_selectedRemap = -1;

		UiModule::Margin* m_spritePreviewMargin = nullptr;
		UiModule::Sprite* m_spritePreview = nullptr;
		UiModule::SpriteOptions m_spritePreviewOptions;
	};
}

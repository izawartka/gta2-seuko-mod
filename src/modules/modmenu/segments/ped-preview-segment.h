#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../utils/spawn-ped-utils.h"

namespace ModMenuModule {
	static constexpr Game::SCR_f PED_SPRITE_PREVIEW_SCALE = Game::Utils::FromFloat(2.0f);
	static constexpr uint16_t PED_SPRITE_PREVIEW_SPRITE_ID_BASE = 53;
	static constexpr uint16_t PED_SPRITE_PREVIEW_SPRITE_ID_SHAPE_OFFSET = 158;

	class PedPreviewSegment : public SegmentBase {
	public:
		PedPreviewSegment();
		virtual ~PedPreviewSegment();

		void SetPedData(const Utils::SpawnPed::SpawnPedData& pedData);

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		void UpdateSpritePreview();

		Utils::SpawnPed::SpawnPedData m_data;
		UiModule::Sprite* m_spritePreview = nullptr;
		UiModule::SpriteOptions m_spritePreviewOptions = {};
	};
}

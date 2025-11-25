#pragma once
#include "../common.h"
#include "../segment-base.h"

namespace ModMenuModule {
	static constexpr Game::SCR_f SPAWN_VEHICLE_SPRITE_PREVIEW_SCALE = Game::Utils::FromFloat(1.25f);

	struct SpawnVehicleSegmentData {
		Game::CAR_MODEL4 model;
		short remap;
		Game::PALETTE_BASE palette;
	};

	class SpawnVehicleSegment : public Segment<SpawnVehicleSegmentData> {
	public:
		SpawnVehicleSegment() = default;
		SpawnVehicleSegment(const std::string& persistencePrefix);
		virtual ~SpawnVehicleSegment();

		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		virtual std::optional<SpawnVehicleSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const SpawnVehicleSegmentData& data) override;

	private:
		void UpdateSpritePreview();
		void UpdateSpritePreviewMargin();

		UiModule::SelectController<Game::CAR_MODEL4>* m_modelController = nullptr;
		UiModule::SelectController<std::tuple<short, Game::PALETTE_BASE>>* m_remapController = nullptr;

		UiModule::Margin* m_spritePreviewMargin = nullptr;
		UiModule::Sprite* m_spritePreview = nullptr;
		UiModule::SpriteOptions m_spritePreviewOptions;
		std::string m_persistencePrefix = "";
	};
}

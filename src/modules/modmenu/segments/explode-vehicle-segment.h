#pragma once
#include "../common.h"
#include "../segment-base.h"

namespace ModMenuModule {
	struct ExplodeVehicleSegmentData {
		Game::EXPLOSION_SIZE explosionSize;
	};

	class ExplodeVehicleSegment : public Segment<ExplodeVehicleSegmentData> {
	public:
		ExplodeVehicleSegment() = default;
		ExplodeVehicleSegment(std::string_view persistencePrefix);
		virtual ~ExplodeVehicleSegment();

		virtual std::optional<ExplodeVehicleSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const ExplodeVehicleSegmentData& data) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		UiModule::SelectController<Game::EXPLOSION_SIZE>* m_explosionSizeController = nullptr;

		std::string m_persistencePrefix = "";
	};
}

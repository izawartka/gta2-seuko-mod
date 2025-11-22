#pragma once
#include "../common.h"
#include "../segment-base.h"

namespace ModMenuModule {
	struct ExplodeAllVehiclesSegmentData {
		bool excludePlayerVehicle;
		Game::EXPLOSION_SIZE explosionSize;
	};

	class ExplodeAllVehiclesSegment : public Segment<ExplodeAllVehiclesSegmentData> {
	public:
		ExplodeAllVehiclesSegment() = default;
		ExplodeAllVehiclesSegment(const std::string& persistencePrefix);
		virtual ~ExplodeAllVehiclesSegment();

		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		virtual std::optional<ExplodeAllVehiclesSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const ExplodeAllVehiclesSegmentData& data) override;

	private:
		UiModule::SelectController<bool>* m_excludePlayerVehicleController = nullptr;
		UiModule::SelectController<Game::EXPLOSION_SIZE>* m_explosionSizeController = nullptr;

		std::string m_persistencePrefix = "";
	};
}

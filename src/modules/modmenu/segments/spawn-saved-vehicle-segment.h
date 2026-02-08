#pragma once
#include "../common.h"
#include "../segment-base.h"

namespace ModMenuModule {
	struct SpawnSavedVehicleSegmentData {
		std::wstring savedCarName;
	};

	class SpawnSavedVehicleSegment : public Segment<SpawnSavedVehicleSegmentData> {
	public:
		SpawnSavedVehicleSegment() = default;
		SpawnSavedVehicleSegment(const std::string& persistencePrefix);
		virtual ~SpawnSavedVehicleSegment();

		virtual bool ValidateSegment() const override;

		virtual std::optional<SpawnSavedVehicleSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const SpawnSavedVehicleSegmentData& data) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		UiModule::SelectController<std::wstring>* m_savedCarNameController = nullptr;
		std::string m_persistencePrefix = "";
	};
}

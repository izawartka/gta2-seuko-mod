#pragma once
#include "../common.h"
#include "../segment-base.h"

namespace ModMenuModule {
	struct FixVehicleSegmentData {
		bool doExtinguish;
		bool doFixEngineDamage;
		bool doFixVisualDamage;
	};

	class FixVehicleSegment : public Segment<FixVehicleSegmentData> {
	public:
		FixVehicleSegment() = default;
		FixVehicleSegment(const std::string& persistencePrefix);
		virtual ~FixVehicleSegment();

		virtual bool ValidateSegment() const override;

		virtual std::optional<FixVehicleSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const FixVehicleSegmentData& data) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		UiModule::SelectController<bool>* m_doExtinguishController = nullptr;
		UiModule::SelectController<bool>* m_doFixEngineDamageController = nullptr;
		UiModule::SelectController<bool>* m_doFixVisualDamageController = nullptr;

		std::string m_persistencePrefix = "";
	};
}

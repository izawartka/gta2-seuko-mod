#pragma once
#include "../common.h"
#include "../segment-base.h"

namespace ModMenuModule {
	struct GetPowerupSegmentData {
		Game::POWERUP_TYPE powerupType;
	};

	class GetPowerupSegment : public Segment<GetPowerupSegmentData> {
	public:
		GetPowerupSegment() = default;
		GetPowerupSegment(const std::string& persistencePrefix);
		virtual ~GetPowerupSegment();

		virtual std::optional<GetPowerupSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const GetPowerupSegmentData& data) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		UiModule::SelectController<Game::POWERUP_TYPE>* m_powerupTypeController = nullptr;
		std::string m_persistencePrefix = "";
	};
}

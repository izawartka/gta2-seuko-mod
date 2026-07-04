#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../events/position-store-entries-update.h"
#include "../cheats/position-store.h"
#include "position-rotation-segment.h"

namespace ModMenuModule {
	using PositionMenuSegmentData = PositionRotationSegmentData;

	class PositionMenuSegment : public Segment<PositionMenuSegmentData>, public Core::EventListenerSupport {
	public:
		PositionMenuSegment();
		PositionMenuSegment(std::string_view persistenceKey);
		PositionMenuSegment(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey);
		virtual ~PositionMenuSegment();

		virtual std::optional<PositionMenuSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const PositionMenuSegmentData& data) override;

		bool OnPassedMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnPositionStoreEntriesUpdate(ModMenuModule::PositionStoreEntriesUpdateEvent& event);
		bool UpdateTexts();

		UiModule::VertCont* m_positionCont = nullptr;
		UiModule::Text* m_modeText = nullptr;
		UiModule::Text* m_valueText = nullptr;
		UiModule::MenuItemId m_positionContMenuItemId = -1;
		PositionStoreCheat::PositionId m_positionId = 0;
		std::string m_persistenceKey = "";
		bool m_ownsPositionId = false;
	};
}

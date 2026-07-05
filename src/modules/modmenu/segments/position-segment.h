#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../events/position-store-entries-update.h"
#include "../cheats/position-store.h"

namespace ModMenuModule {
	// Currently unused
	struct PositionSegmentData {
		bool updateFromPlayer = true;
		Game::SCR_Vector3 position = { 0, 0, 0 };
		bool autoZ = true;
	};

	// Currently unused
	class PositionSegment : public Segment<PositionSegmentData>, public Core::EventListenerSupport {
	public:
		PositionSegment();
		PositionSegment(std::string_view persistenceKey);
		PositionSegment(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey);
		virtual ~PositionSegment();

		virtual std::optional<PositionSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const PositionSegmentData& data) override;

		bool SetDoUpdatePosition(bool doUpdateFromPlayer);
		bool GetDoUpdatePosition() const;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnPositionStoreEntriesUpdate(ModMenuModule::PositionStoreEntriesUpdateEvent& event);

		static Game::SCR_f ClampCoord(Game::SCR_f value, bool isZCoord);

		bool UpdateControllers();
		bool OnDoUpdatePositionControllerSave(bool newValue);
		void OnCoordControllerSave(size_t coordIndex, Game::SCR_f newValue);
		void OnAutoZControllerSave(bool newValue);
		bool UpdateEntry(std::function<bool(PositionStoreEntry&)> updateFunc);

		UiModule::SelectController<bool>* m_doUpdatePositionController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_xController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_yController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_zController = nullptr;
		UiModule::SelectController<bool>* m_autoZController = nullptr;
		PositionStoreCheat::PositionId m_positionId = 0;
		std::string m_persistenceKey = "";
		bool m_ownsPositionId = false;
	};
}

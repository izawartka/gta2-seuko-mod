#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../events/position-store-entries-update.h"
#include "../cheats/position-store.h"

namespace ModMenuModule {
	struct PositionRotationSegmentData {
		bool updateFromPlayer = true;
		Game::SCR_Vector3 position = { 0, 0, 0 };
		bool autoZ = true;
		short rotation = 0;
	};

	class PositionRotationSegment : public Segment<PositionRotationSegmentData>, public Core::EventListenerSupport {
	public:
		PositionRotationSegment();
		PositionRotationSegment(std::string_view persistencePrefix);
		PositionRotationSegment(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey);
		virtual ~PositionRotationSegment();

		virtual std::optional<PositionRotationSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const PositionRotationSegmentData& data) override;

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
		void OnRotationControllerSave(short newValue);
		bool UpdateEntry(std::function<bool(PositionStoreEntry&)> updateFunc);

		UiModule::SelectController<bool>* m_doUpdatePositionController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_xController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_yController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_zController = nullptr;
		UiModule::SelectController<bool>* m_autoZController = nullptr;
		UiModule::EditableController<short>* m_rotationController = nullptr;
		PositionStoreCheat::PositionId m_positionId = 0;
		std::string m_persistenceKey = "";
		bool m_ownsPositionId = false;
	};
}

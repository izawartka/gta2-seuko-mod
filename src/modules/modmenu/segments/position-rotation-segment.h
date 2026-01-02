#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../events/player-pos-update.h"
#include "../events/player-rot-update.h"

namespace ModMenuModule {
	struct PositionRotationSegmentData {
		bool updateFromPlayer = true;
		Game::SCR_Vector3 position;
		bool autoZ = true;
		short rotation = 0;
	};

	class PositionRotationSegment : public Segment<PositionRotationSegmentData>, public Core::EventListenerSupport {
	public:
		PositionRotationSegment() = default;
		PositionRotationSegment(const std::string& persistencePrefix);
		virtual ~PositionRotationSegment();

		virtual std::optional<PositionRotationSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const PositionRotationSegmentData& data) override;

		void SetDoUpdatePosition(bool doUpdateFromPlayer);
		bool GetDoUpdatePosition() const { return m_doUpdatePosition; }

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		static bool ValidateCoord(Game::SCR_f value, bool isZCoord);

		void SetCoordControllerValues(const Game::SCR_Vector3& position);
		std::optional<Game::SCR_Vector3> GetCoordControllerValues() const;

		void OnPlayerPosUpdate(ModMenuModule::PlayerPosUpdateEvent& event);
		void OnPlayerRotUpdate(ModMenuModule::PlayerRotUpdateEvent& event);
		void ApplyAutoZIfNeeded();
		void ForceUpdatePosition();
		void OnDoUpdatePositionControllerSave(bool newValue);
		void OnCoordControllerSave(bool isZCoord);
		void OnAutoZControllerSave(bool newValue);

		UiModule::SelectController<bool>* m_doUpdatePositionController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_xController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_yController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_zController = nullptr;
		UiModule::SelectController<bool>* m_autoZController = nullptr;
		UiModule::EditableController<short>* m_rotationController = nullptr;
		bool m_doUpdatePosition = true;
		std::string m_persistencePrefix = "";
	};
}

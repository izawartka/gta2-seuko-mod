#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../cheat-support.h"
#include "../events/player-pos-update.h"

namespace ModMenuModule {
	struct PositionSegmentData {
		bool updateFromPlayer = true;
		Game::SCR_Vector3 position;
		bool autoZ = true;
	};

	class PositionSegment : public Segment<PositionSegmentData>, public Core::EventListenerSupport, public CheatSupport {
	public:
		PositionSegment() = default;
		PositionSegment(const std::string& persistencePrefix);
		virtual ~PositionSegment();

		virtual std::optional<PositionSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const PositionSegmentData& data) override;

		void SetDoUpdateFromPlayer(bool doUpdateFromPlayer);
		bool GetDoUpdateFromPlayer() const { return m_doUpdateFromPlayer; }

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void SetCoordControllerValues(const Game::SCR_Vector3& position);
		std::optional<Game::SCR_Vector3> GetCoordControllerValues() const;

		void OnPlayerPosUpdate(ModMenuModule::PlayerPosUpdateEvent& event);
		void ApplyAutoZIfNeeded();
		void ForceUpdateFromPlayer();
		void OnUpdateFromPlayerControllerSave(bool newValue);
		void OnCoordControllerSave(bool isZCoord);
		void OnAutoZControllerSave(bool newValue);

		UiModule::SelectController<bool>* m_updateFromPlayerController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_xController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_yController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_zController = nullptr;
		UiModule::SelectController<bool>* m_autoZController = nullptr;
		bool m_doUpdateFromPlayer = true;
		std::string m_persistencePrefix = "";
	};
}

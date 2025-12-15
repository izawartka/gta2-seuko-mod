#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../cheat-support.h"
#include "../events/player-pos-update.h"

namespace ModMenuModule {
	struct PositionSegmentData {
		Game::SCR_f x = Game::Utils::FromFloat(0.0f);
		Game::SCR_f y = Game::Utils::FromFloat(0.0f);
		Game::SCR_f z = Game::Utils::FromFloat(0.0f);
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

		void OnPlayerPosUpdate(ModMenuModule::PlayerPosUpdateEvent& event);
		void SetControllerValues(Game::SCR_f x, Game::SCR_f y, Game::SCR_f z);
		void ApplyAutoZIfNeeded();
		void ForceUpdateFromPlayer();
		void OnCoordControllerSave(bool isZCoord);
		void OnAutoZControllerSave(bool newValue);

		UiModule::EditableController<Game::SCR_f>* m_xController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_yController = nullptr;
		UiModule::EditableController<Game::SCR_f>* m_zController = nullptr;
		UiModule::SelectController<bool>* m_autoZController = nullptr;
		bool m_doUpdateFromPlayer = false;

		std::string m_persistencePrefix = "";
	};
}

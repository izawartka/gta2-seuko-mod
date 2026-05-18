#pragma once
#include "../../common.h"
#include "../../../../events/game-end.h"
#include "../../cheat-base.h"

namespace ModMenuModule {
	class FreecamCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		FreecamCheat();
		virtual ~FreecamCheat() override;
		static FreecamCheat* GetInstance();

		void ResetAndDisable();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnControlStateChange(const std::optional<Game::PLAYER_CONTROL_STATE>& oldValue, const std::optional<Game::PLAYER_CONTROL_STATE>& newValue);
		void OnGameEnd(GameEndEvent& event);

		void ResetAndDisableCheckAndProceed();

		static FreecamCheat* m_instance;

		Core::Resolver<Game::PLAYER_CONTROL_STATE*> m_controlStateResolver = nullptr;
		std::optional<Game::PLAYER_CONTROL_STATE> m_originalControlState = std::nullopt;
		Core::Watched<Game::PLAYER_CONTROL_STATE>* m_watchedControlState = nullptr;

		bool m_isDisabling = false;
		bool m_resetAndDisableDone = false;
	};
}

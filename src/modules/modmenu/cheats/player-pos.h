#pragma once
#include "../common.h"
#include "../../../events/game-tick.h"
#include "../../../events/game-end.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	class PlayerPosCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		PlayerPosCheat();
		virtual ~PlayerPosCheat();

		void Teleport(Game::SCR_f x, Game::SCR_f y);
		void Teleport(Game::SCR_f x, Game::SCR_f y, Game::SCR_f z);
		void Teleport(const Game::SCR_Vector2& position);
		void Teleport(const Game::SCR_Vector3& position);

		const std::optional<Game::SCR_Vector3>& GetLastPosition() const { return m_position; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		void OnPreGameTick(PreGameTickEvent& event);
		void OnGameEnd(GameEndEvent& event);

		void UpdateCameraSync();

		std::optional<Game::SCR_Vector3> m_position = std::nullopt;
		std::optional<Game::SCR_Vector3> m_teleportPosition = std::nullopt;
		bool m_teleportAutoZ = false;
		int m_cameraSyncTimer = 0;
	};
}

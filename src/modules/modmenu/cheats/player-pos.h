#pragma once
#include "../common.h"
#include "../../../events/game-tick.h"
#include "../../../events/game-end.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	using PlayerPosCheatTeleportCallback = std::function<void(bool success)>;

	class PlayerPosCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		PlayerPosCheat();
		virtual ~PlayerPosCheat() override;
		static PlayerPosCheat* GetInstance();

		void Teleport(Game::SCR_f x, Game::SCR_f y, PlayerPosCheatTeleportCallback callback = nullptr);
		void Teleport(Game::SCR_f x, Game::SCR_f y, Game::SCR_f z, PlayerPosCheatTeleportCallback callback = nullptr);
		void Teleport(const Game::SCR_Vector2& position, PlayerPosCheatTeleportCallback callback = nullptr);
		void Teleport(const Game::SCR_Vector3& position, PlayerPosCheatTeleportCallback callback = nullptr);

		const std::optional<Game::SCR_Vector3>& GetLastPosition() const { return m_position; }
		const std::optional<short>& GetLastRotation() const { return m_rotation; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		void OnPreGameTick(PreGameTickEvent& event);
		void OnGameEnd(GameEndEvent& event);

		void UpdateCameraSync();

		static PlayerPosCheat* m_instance;
		std::optional<Game::SCR_Vector3> m_position = std::nullopt;
		std::optional<short> m_rotation = std::nullopt;
		std::optional<Game::SCR_Vector3> m_teleportPosition = std::nullopt;
		PlayerPosCheatTeleportCallback m_teleportCallback = nullptr;
		bool m_teleportAutoZ = false;
		int m_cameraSyncTimer = 0;
	};
}

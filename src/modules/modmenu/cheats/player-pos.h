#pragma once
#include "../common.h"
#include "../../../events/game-tick.h"
#include "../../../events/game-end.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	struct PlayerPosCheatPos {
		Game::SCR_f x;
		Game::SCR_f y;
		Game::SCR_f z;

		PlayerPosCheatPos(Game::SCR_f x, Game::SCR_f y, Game::SCR_f z)
			: x(x), y(y), z(z) {
		}

		bool operator==(const PlayerPosCheatPos& other) const {
			return x == other.x && y == other.y && z == other.z;
		}

		bool operator!=(const PlayerPosCheatPos& other) const {
			return !(*this == other);
		}
	};

	class PlayerPosCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		PlayerPosCheat();
		virtual ~PlayerPosCheat();

		void Teleport(Game::SCR_f x, Game::SCR_f y);
		void Teleport(Game::SCR_f x, Game::SCR_f y, Game::SCR_f z);
		const std::optional<PlayerPosCheatPos>& GetLastPosition() const { return m_position; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		void OnPreGameTick(PreGameTickEvent& event);
		void OnGameEnd(GameEndEvent& event);

		void UpdateCameraSync();

		std::optional<PlayerPosCheatPos> m_position = std::nullopt;
		std::optional<PlayerPosCheatPos> m_teleportPosition = std::nullopt;
		bool m_teleportAutoZ = false;
		int m_cameraSyncTimer = 0;
	};
}

#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../../../../events/camera-pos-apply.h"
#include "../../../../events/game-start.h"

namespace ModMenuModule {
	enum class CameraPosCheatMode {
		Unmodified = 0,
		LockTargetAt,
		IncrementTargetBy
	};

	struct CameraPosCheatCoordinate {
		CameraPosCheatMode mode = CameraPosCheatMode::Unmodified;
		Game::SCR_f value = Game::Utils::FromFloat(0.0f);
		bool lockAtTarget = false;
	};

	struct CameraPosCheatOptions {
		CameraPosCheatCoordinate x;
		CameraPosCheatCoordinate y;
		CameraPosCheatCoordinate z;
		CameraPosCheatCoordinate zoom;
		bool reverseZMinLock = false;
	};

	struct CameraPosCheatPosition {
		Game::SCR_f x = Game::Utils::FromFloat(0.0f);
		Game::SCR_f y = Game::Utils::FromFloat(0.0f);
		Game::SCR_f z = Game::Utils::FromFloat(0.0f);
		Game::SCR_f zoom = Game::Utils::FromFloat(1.0f);
	};

	class CameraPosCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		CameraPosCheat();
		virtual ~CameraPosCheat();

		static const std::vector<CameraPosCheatMode> GetAllCameraPosCheatModes();

		void ResetPosOptions(); // resets all position options to unmodified
		void LockAtCurrentPos(); // locks camera target to current camera position
		void SnapToTargetPos(); // snaps camera position to target camera position
		void SnapAndDisable(); // snaps camera position to original target camera position and disables the cheat

		void SetOptions(const CameraPosCheatOptions& options);
		const CameraPosCheatOptions& GetOptions() const { return m_options; }

		const std::optional<CameraPosCheatPosition>& GetLastPosition() const { return m_lastPosition; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnCameraPosApply(CameraPosApplyEvent& event);
		void OnGameStart(GameStartEvent& event);
		void ApplyCoordinate(CameraPosCheatCoordinate& coord, Game::SCR_f& camCoord, Game::SCR_f& camCoordTarget2) const;
		void ApplyReverseZMinLock(Game::Camera* camera) const;

		void SaveToPersistence() const;
		void LoadFromPersistence();

		CameraPosCheatOptions m_options;
		bool m_lockAtCurrentRequested = false;
		bool m_snapToTargetRequested = false;
		bool m_snapAndDisableRequested = false;
		std::optional<CameraPosCheatPosition> m_lastPosition = std::nullopt;
	};
}

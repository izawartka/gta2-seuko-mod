#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../../../../events/camera-pos-apply.h"
#include "../../../../events/game-start.h"
#include "../../../../events/game-end.h"

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

		bool SmartEquals(const CameraPosCheatCoordinate& other) const {
			return mode == other.mode && (mode == CameraPosCheatMode::Unmodified || value == other.value) && lockAtTarget == other.lockAtTarget;
		}

		bool operator==(const CameraPosCheatCoordinate& other) const {
			return mode == other.mode && value == other.value && lockAtTarget == other.lockAtTarget;
		}

		bool operator!=(const CameraPosCheatCoordinate& other) const {
			return !(*this == other);
		}
	};

	struct CameraPosCheatOptions {
		CameraPosCheatCoordinate x;
		CameraPosCheatCoordinate y;
		CameraPosCheatCoordinate z;
		CameraPosCheatCoordinate zoom;
		bool reverseZMinLock = false;

		bool SmartEquals(const CameraPosCheatOptions& other) const {
			return x.SmartEquals(other.x) && y.SmartEquals(other.y) && z.SmartEquals(other.z) && zoom.SmartEquals(other.zoom) && reverseZMinLock == other.reverseZMinLock;
		}

		bool operator==(const CameraPosCheatOptions& other) const {
			return x == other.x && y == other.y && z == other.z && zoom == other.zoom && reverseZMinLock == other.reverseZMinLock;
		}

		bool operator!=(const CameraPosCheatOptions& other) const {
			return !(*this == other);
		}
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

		const std::optional<Game::CameraPos>& GetLastPosition() const { return m_lastPosition; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnCameraPosApply(CameraPosApplyEvent& event);
		void OnGameStart(GameStartEvent& event);
		void OnGameEnd(GameEndEvent& event);
		void ApplyCoordinate(CameraPosCheatCoordinate& coord, Game::SCR_f& camCoord, Game::SCR_f& camCoordTarget2) const;
		void ApplyReverseZMinLock(Game::Camera* camera) const;

		void SaveToPersistence() const;
		void LoadFromPersistence();

		CameraPosCheatOptions m_options;
		bool m_lockAtCurrentRequested = false;
		bool m_snapToTargetRequested = false;
		bool m_snapAndDisableRequested = false;
		std::optional<Game::CameraPos> m_lastPosition = std::nullopt;
	};
}

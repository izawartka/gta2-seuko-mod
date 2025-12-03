#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../../../../events/game-tick.h"

namespace ModMenuModule {
	struct CameraPosOptions {
		std::optional<Game::SCR_f> lockedX = std::nullopt;
		std::optional<Game::SCR_f> lockedY = std::nullopt;
		std::optional<Game::SCR_f> lockedZ = std::nullopt;
	};

	class CameraPosCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		CameraPosCheat();
		virtual ~CameraPosCheat();

		void SetFromCurrentPos(); // locks camera to current position
		void SetFromOriginalTargetPos(); // jumps and locks camera to original target position
		void SetOptions(const CameraPosOptions& options);
		const CameraPosOptions& GetOptions() const { return m_options; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnGameTick(GameTickEvent& event);

		CameraPosOptions m_options;
		Core::Resolver<Game::Camera*> m_camera1Resolver = nullptr;
		Core::Resolver<Game::Camera*> m_camera2Resolver = nullptr;
		bool m_setFromCurrentRequested = false;
		bool m_setFromOriginalTargetRequested = false;
	};
}

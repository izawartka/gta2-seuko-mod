#pragma once
#include "../common.h"
#include "../../../events/game-end.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	class InvisibilityCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		InvisibilityCheat();
		virtual ~InvisibilityCheat() override;
		static InvisibilityCheat* GetInstance();

		void ResetAndDisable();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnBitstateUpdate(const std::optional<Game::PED_BIT_STATE>& oldValue, const std::optional<Game::PED_BIT_STATE>& newValue);
		void OnInvisibilityUpdate(const std::optional<Game::SPRITE_INVISIBILITY>& oldValue, const std::optional<Game::SPRITE_INVISIBILITY>& newValue);
		void OnGameEnd(GameEndEvent& event);

		static InvisibilityCheat* m_instance;

		Core::Resolver<Game::PED_BIT_STATE*> m_bitstateResolver = nullptr;
		Core::Watched<Game::PED_BIT_STATE>* m_watchedBitstate = nullptr;
		Core::Resolver<Game::SPRITE_INVISIBILITY*> m_invisibilityResolver = nullptr;
		Core::Watched<Game::SPRITE_INVISIBILITY>* m_watchedInvisibility = nullptr;

		bool m_isDisabling = false;
	};
}

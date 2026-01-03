#pragma once
#include "../common.h"
#include "../../../events/game-end.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	class PlayerAppearanceCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		PlayerAppearanceCheat();
		virtual ~PlayerAppearanceCheat() override;
		static PlayerAppearanceCheat* GetInstance();

		void SetRemapLocked(bool locked);
		bool IsRemapLocked() const;
		void SetRemap(Game::PED_REMAP remap);
		std::optional<Game::PED_REMAP> GetRemap() const;

		void SetGraphicTypeLocked(bool locked);
		bool IsGraphicTypeLocked() const;
		void SetGraphicType(Game::PED_GRAPHIC_TYPE graphicType);
		std::optional<Game::PED_GRAPHIC_TYPE> GetGraphicType() const;

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnPedRemapUpdate(const std::optional<Game::PED_REMAP>& oldValue, const std::optional<Game::PED_REMAP>& newValue);
		void OnPedGraphicTypeUpdate(const std::optional<Game::PED_GRAPHIC_TYPE>& oldValue, const std::optional<Game::PED_GRAPHIC_TYPE>& newValue);
		void OnGameEnd(GameEndEvent& event);

		void SaveToPersistence() const;
		void LoadFromPersistence();

		static PlayerAppearanceCheat* m_instance;

		Core::Resolver<Game::PED_REMAP*> m_pedRemapResolver = nullptr;
		Core::Resolver<Game::PED_GRAPHIC_TYPE*> m_pedGraphicTypeResolver = nullptr;
		std::optional<Game::PED_REMAP> m_lockedRemap = std::nullopt;

		Core::Watched<Game::PED_REMAP>* m_watchedPedRemap = nullptr;
		Core::Watched<Game::PED_GRAPHIC_TYPE>* m_watchedPedGraphicType = nullptr;
		std::optional<Game::PED_GRAPHIC_TYPE> m_lockedGraphicType = std::nullopt;
	};
}

#pragma once
#include "../common.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	enum class LastCarState {
		NoCar,
		InCar,
		OutOfCar
	};

	class LastCarCheat : public CheatBase {
	public:
		LastCarCheat();
		virtual ~LastCarCheat() override;
		static LastCarCheat* GetInstance();

		void ResetLastCar();
		Game::Car* GetLastCar() const { return m_lastCar; }
		LastCarState GetLastCarState() const { return m_state; }
		std::wstring GetLastCarStateMenuName() const;

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		void OnCurrentCarUpdate(const std::optional<Game::Car*>& oldValue, const std::optional<Game::Car*>& newValue);
		void OnLastCarIdUpdate(const std::optional<Game::uint>& oldValue, const std::optional<Game::uint>& newValue);
		void SetState(LastCarState state);

		static LastCarCheat* m_instance;
		Core::Resolver<Game::Car*> m_currentCarResolver = nullptr;
		Core::Watched<Game::Car*, Game::Car*>* m_watchedCurrentCar = nullptr;
		Core::Resolver<Game::uint*> m_lastCarIdResolver = nullptr;
		Core::Watched<Game::uint>* m_watchedLastCarId = nullptr;
		Game::Car* m_lastCar = nullptr;
		LastCarState m_state = LastCarState::NoCar;
		Game::uint m_lastCarId = 0;
	};
}

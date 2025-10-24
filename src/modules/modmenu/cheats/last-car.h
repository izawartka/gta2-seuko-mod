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
		virtual ~LastCarCheat();

		void ResetLastCar();
		Game::Car* GetLastCar() const { return m_lastCar; }
		LastCarState GetLastCarState() const { return m_lastState; }
		std::wstring GetLastCarStateMenuName() const;

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		void OnValueUpdate(std::optional<Game::Car*> oldValue, std::optional<Game::Car*> newValue);
		void SetState(LastCarState state);

		Core::Resolver<Game::Car*> m_currentCarResolver = nullptr;
		Core::Watched<Game::Car*>* m_watchedCurrentCar = nullptr;
		Game::Car* m_lastCar = nullptr;
		LastCarState m_lastState = LastCarState::NoCar;
	};
}

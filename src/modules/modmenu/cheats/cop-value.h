#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/cop-value-change.h"
#include "../../../events/game-end.h"

namespace ModMenuModule {
	class CopValueCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		CopValueCheat();
		virtual ~CopValueCheat() override;
		static CopValueCheat* GetInstance();

		void SetCopValueLocked(bool locked);
		bool IsCopValueLocked() const;

		void SetCopValue(short copValue);
		const std::optional<short>& GetCopValue() const;

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void UpdateCopValueChangeListener();
		void OnValueUpdate(const std::optional<short>& oldValue, const std::optional<short>& newValue);
		void OnCopValueChange(CopValueChangeEvent& event);
		void OnGameEnd(GameEndEvent& event);

		void SaveToPersistence() const;
		void LoadFromPersistence();

		static CopValueCheat* m_instance;
		Core::Resolver<short*> m_copValueResolver = nullptr;
		Core::Watched<short>* m_watchedCopValue = nullptr;
		std::optional<short> m_lockedCopValue = std::nullopt;
	};
}

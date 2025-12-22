#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/cop-value-change.h"

namespace ModMenuModule {
	class CopValueCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		CopValueCheat();
		virtual ~CopValueCheat() override;
		static CopValueCheat* GetInstance();

		void SetCopValueFrozen(bool frozen);
		bool IsCopValueFrozen() const;

		void SetCopValue(short copValue);
		const std::optional<short>& GetCopValue() const;

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		void UpdateCopValueChangeListener();
		void OnValueUpdate(const std::optional<short>& oldValue, const std::optional<short>& newValue);
		void OnCopValueChange(CopValueChangeEvent& event);

		void SaveToPersistence() const;
		void LoadFromPersistence();

		static CopValueCheat* m_instance;
		Core::Resolver<short*> m_copValueResolver = nullptr;
		Core::Watched<short>* m_watchedCopValue = nullptr;
		bool m_isCopValueFrozen = false;
	};
}

#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/cop-value-change.h"

namespace ModMenuModule {
	class FreezeCopValueCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		FreezeCopValueCheat();
		virtual ~FreezeCopValueCheat() override;
		static FreezeCopValueCheat* GetInstance();

		void SetFrozenCopValue(short copValue);
		const std::optional<short>& GetFrozenCopValue() const { return m_copValue; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnValueUpdate(const std::optional<short>& oldValue, const std::optional<short>& newValue);
		void OnCopValueChange(CopValueChangeEvent& event);

		static FreezeCopValueCheat* m_instance;
		Core::Resolver<short*> m_copValueResolver = nullptr;
		Core::Watched<short>* m_watchedCopValue = nullptr;
		std::optional<short> m_copValue = std::nullopt;
	};
}

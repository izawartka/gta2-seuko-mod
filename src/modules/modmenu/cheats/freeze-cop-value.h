#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/cop-value-change.h"

namespace ModMenuModule {
	class FreezeCopValueCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		FreezeCopValueCheat();
		virtual ~FreezeCopValueCheat();

		void SetCopValue(short copValue);

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnValueUpdate(const std::optional<short>& oldValue, const std::optional<short>& newValue);
		void OnCopValueChange(CopValueChangeEvent& event);

		Core::Resolver<short*> m_copValueResolver = nullptr;
		Core::Watched<short>* m_watchedCopValue = nullptr;
		short m_copValue = 0;
		bool m_justEnabled = false;
	};
}

#pragma once
#include "../common.h"
#include "../cheat-base.h"

namespace ModMenuModule {
	class FreezeCopValue : public CheatBase {
	public:
		FreezeCopValue();
		virtual ~FreezeCopValue();

		void SetCopValue(short copValue);

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnValueUpdate(std::optional<short> oldValue, std::optional<short> newValue);

		Core::Resolver<short> m_copValueResolver = nullptr;
		Core::Watched<short>* m_watchedCopValue = nullptr;
		short m_copValue = 0;
		bool m_justEnabled = false;
	};
}

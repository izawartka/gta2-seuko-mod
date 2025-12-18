#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/game-tick.h"
#include "../../../events/game-end.h"

namespace ModMenuModule {
	class SetCopValueCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		SetCopValueCheat();
		virtual ~SetCopValueCheat() override;
		static SetCopValueCheat* GetInstance();

		void SetCopValueAndDisable(short copValue);
		const std::optional<short>& GetPendingValue() const { return m_pendingValue; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnGameTick(GameTickEvent& event);
		void OnGameEnd(GameEndEvent& event);

		static SetCopValueCheat* m_instance;
		Core::Resolver<short*> m_copValueResolver = nullptr;
		std::optional<short> m_pendingValue = std::nullopt;
	};
}

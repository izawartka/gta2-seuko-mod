#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../cheat-support.h"

namespace ModMenuModule {
	class ClearWantedLevelAction : public QuickActionBase, public CheatSupport, public Core::EventListenerSupport {
	public:
		ClearWantedLevelAction();
		virtual ~ClearWantedLevelAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

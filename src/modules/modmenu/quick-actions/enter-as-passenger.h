#pragma once
#include "../common.h"
#include "../quick-action-base.h"

namespace ModMenuModule {
	class EnterAsPassengerAction : public QuickActionBase, public Core::EventListenerSupport {
	public:
		EnterAsPassengerAction();
		virtual ~EnterAsPassengerAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

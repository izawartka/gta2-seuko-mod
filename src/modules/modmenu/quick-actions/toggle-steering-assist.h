#pragma once
#include "../common.h"
#include "../quick-action-base.h"

namespace ModMenuModule {
	class ToggleSteeringAssistAction : public QuickActionBase, public Core::EventListenerSupport {
	public:
		ToggleSteeringAssistAction();
		virtual ~ToggleSteeringAssistAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

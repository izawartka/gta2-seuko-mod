#pragma once
#include "../common.h"
#include "../quick-action-base.h"

namespace ModMenuModule {
	class ToggleInvisibilityAction : public QuickActionBase, public Core::EventListenerSupport {
	public:
		ToggleInvisibilityAction();
		virtual ~ToggleInvisibilityAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

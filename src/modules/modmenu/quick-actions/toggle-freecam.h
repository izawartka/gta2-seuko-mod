#pragma once
#include "../common.h"
#include "../quick-action-base.h"

namespace ModMenuModule {
	class ToggleFreecamAction : public QuickActionBase, public Core::EventListenerSupport {
	public:
		ToggleFreecamAction();
		virtual ~ToggleFreecamAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

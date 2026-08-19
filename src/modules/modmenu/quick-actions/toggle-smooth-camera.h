#pragma once
#include "../common.h"
#include "../quick-action-base.h"

namespace ModMenuModule {
	class ToggleSmoothCameraAction : public QuickActionBase, public Core::EventListenerSupport {
	public:
		ToggleSmoothCameraAction();
		virtual ~ToggleSmoothCameraAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

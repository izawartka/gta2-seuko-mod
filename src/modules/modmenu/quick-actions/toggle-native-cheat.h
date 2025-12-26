#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/toggle-native-cheat-segment.h"

namespace ModMenuModule {
	class ToggleNativeCheatAction : public QuickActionWithSegment<ToggleNativeCheatSegmentData> {
	public:
		ToggleNativeCheatAction();
		virtual ~ToggleNativeCheatAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static ToggleNativeCheatSegment* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;

	private:
		virtual void OnDataChange() override;

		std::wstring m_label;
	};
}

#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/switch-camera-mode-segment.h"

namespace ModMenuModule {
	class SwitchCameraModeAction : public QuickActionWithSegment<SwitchCameraModeSegmentData> {
	public:
		SwitchCameraModeAction();
		virtual ~SwitchCameraModeAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static SwitchCameraModeSegment* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;

		std::vector<uint8_t> SerializeData() const override;
		bool DeserializeData(const std::vector<uint8_t>& data) override;
	};
}

#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/get-powerup-segment.h"

namespace ModMenuModule {
	class GetPowerupAction : public QuickActionWithSegment<GetPowerupSegmentData> {
	public:
		GetPowerupAction();
		virtual ~GetPowerupAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static GetPowerupSegment* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;

	private:
		virtual void OnDataChange() override;

		std::wstring m_label;
	};
}

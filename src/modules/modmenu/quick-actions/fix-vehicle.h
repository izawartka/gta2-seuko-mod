#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/fix-vehicle-segment.h"

namespace ModMenuModule {
	class FixVehicleAction : public QuickActionWithSegment<FixVehicleSegmentData> {
	public:
		FixVehicleAction();
		virtual ~FixVehicleAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static FixVehicleSegment* CreateSegmentInstance();
		static bool ValidateSegmentData(const FixVehicleSegmentData& data);

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

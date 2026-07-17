#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/explode-vehicle-segment.h"

namespace ModMenuModule {
	class ExplodeVehicleAction : public QuickActionWithSegment<ExplodeVehicleSegmentData> {
	public:
		ExplodeVehicleAction();
		virtual ~ExplodeVehicleAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static ExplodeVehicleSegment* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

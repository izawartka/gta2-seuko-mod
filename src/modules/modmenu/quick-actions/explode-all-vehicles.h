#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/explode-all-vehicles-segment.h"

namespace ModMenuModule {
	class ExplodeAllVehiclesAction : public QuickActionWithSegment<ExplodeAllVehiclesSegmentData> {
	public:
		ExplodeAllVehiclesAction();
		virtual ~ExplodeAllVehiclesAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static ExplodeAllVehiclesSegment* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

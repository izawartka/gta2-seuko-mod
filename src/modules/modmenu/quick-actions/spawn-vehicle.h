#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/spawn-vehicle-segment.h"

namespace ModMenuModule {
	class SpawnVehicleAction : public QuickActionWithSegment<SpawnVehicleSegmentData> {
	public:
		SpawnVehicleAction();
		virtual ~SpawnVehicleAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static Segment<SpawnVehicleSegmentData>* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;

	private:
		virtual void OnDataChange() override;
		
		std::wstring m_label;
	};
}

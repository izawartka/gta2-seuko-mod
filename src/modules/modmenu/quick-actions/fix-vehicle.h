#pragma once
#include "../common.h"
#include "../quick-action-base.h"

namespace ModMenuModule {
	class FixVehicleAction : public QuickActionBase {
	public:
		FixVehicleAction();
		virtual ~FixVehicleAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();

		void Execute() override;
		const std::wstring& GetLabel() const override;
	};
}

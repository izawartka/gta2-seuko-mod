#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/spawn-object-segment.h"

namespace ModMenuModule {
	class SpawnObjectAction : public QuickActionWithSegment<SpawnObjectSegmentData> {
	public:
		SpawnObjectAction();
		virtual ~SpawnObjectAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static SpawnObjectSegment* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;

	private:
		virtual void OnDataChange() override;

		std::wstring m_label;
	};
}

#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/spawn-ped-action-segment.h"

namespace ModMenuModule {
	class SpawnCharacterAction : public QuickActionWithSegment<SpawnPedActionSegmentData> {
	public:
		SpawnCharacterAction();
		virtual ~SpawnCharacterAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static SpawnPedActionSegment* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;

		std::vector<uint8_t> SerializeData() const override;
		bool DeserializeData(const std::vector<uint8_t>& data) override;

	private:
		virtual void OnDataChange() override;

		std::wstring m_label;
	};
}

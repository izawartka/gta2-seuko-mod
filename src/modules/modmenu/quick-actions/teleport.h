#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../segments/position-rotation-segment.h"

namespace ModMenuModule {
	struct TeleportActionData {
		Game::SCR_Vector3 position;
		short rotation;
	};

	class TeleportAction : public QuickActionWithSegment<PositionRotationSegmentData, TeleportActionData> {
	public:
		TeleportAction();
		virtual ~TeleportAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static PositionRotationSegment* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;

		virtual std::optional<TeleportActionData> SegmentDataToData(const PositionRotationSegmentData& segmentData) override;
		virtual std::optional<PositionRotationSegmentData> DataToSegmentData(const TeleportActionData& data) override;

		virtual bool HandleInvalidDataSize(std::vector<uint8_t>& data) override;

	private:
		virtual void OnDataChange() override;

		std::wstring m_label;
	};
}

#pragma once
#include "../common.h"
#include "../quick-action-base.h"
#include "../cheat-support.h"
#include "../segments/position-segment.h"

namespace ModMenuModule {
	class TeleportAction : public QuickActionWithSegment<PositionSegmentData, Game::SCR_Vector3>, public CheatSupport {
	public:
		TeleportAction();
		virtual ~TeleportAction();
		static const std::string& GetTypeId();
		static const std::wstring& GetTypeLabel();
		static PositionSegment* CreateSegmentInstance();

		void Execute() override;
		const std::wstring& GetLabel() const override;

		virtual std::optional<Game::SCR_Vector3> SegmentDataToData(const PositionSegmentData& segmentData) override;
		virtual std::optional<PositionSegmentData> DataToSegmentData(const Game::SCR_Vector3& data) override;

	private:
		virtual void OnDataChange() override;

		std::wstring m_label;
	};
}

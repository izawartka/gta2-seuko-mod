#pragma once
#include "common.h"
#include "segment-base.h"

namespace ModMenuModule {
	class SegmentSupport {
	protected:
		SegmentSupport() = default;
		virtual ~SegmentSupport();
		SegmentSupport(const SegmentSupport&) = delete;
		SegmentSupport& operator=(const SegmentSupport&) = delete;
		SegmentSupport(SegmentSupport&&) = delete;
		SegmentSupport& operator=(SegmentSupport&&) = delete;

		template<typename SegmentT, typename... Args>
		SegmentT* CreateSegment(Args&&... args) {
			static_assert(std::is_base_of_v<SegmentBase, SegmentT>, "SegmentT must be derived from SegmentBase");
			SegmentT* segment = new SegmentT(std::forward<Args>(args)...);
			AddSegment(segment);
			return segment;
		}

		template<typename SegmentT, typename... Args>
		SegmentT* CreateAttachSetVisibleSegment(ModMenuModule::MenuBase* menu, UiModule::Component* parent, Args&&... args) {
			static_assert(std::is_base_of_v<SegmentBase, SegmentT>, "SegmentT must be derived from SegmentBase");
			SegmentT* segment = new SegmentT(std::forward<Args>(args)...);
			AddAttachSetVisibleSegment(segment, menu, parent);
			return segment;
		}

		void AddSegment(SegmentBase* segment);
		void AddAttachSetVisibleSegment(SegmentBase* segment, ModMenuModule::MenuBase* menu, UiModule::Component* parent);
		bool RemoveSegment(SegmentBase* segment);
		bool DeleteSegment(SegmentBase*& segment);
		void ClearSegments();

		bool AttachSegment(SegmentBase* segment, ModMenuModule::MenuBase* menu, UiModule::Component* parent);
		bool DetachSegment(SegmentBase* segment);
		bool AttachAllSegments(ModMenuModule::MenuBase* menu, UiModule::Component* parent);
		void DetachAllSegments();

		void SetSegmentsVisible(bool visible);

	private:
		struct SegmentEntry {
			SegmentBase* segment;
			bool owned;
		};

		std::vector<SegmentEntry> m_segments;
		bool m_visible = false;
	};
}

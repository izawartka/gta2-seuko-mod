#include "segment-support.h"

ModMenuModule::SegmentSupport::~SegmentSupport()
{
	ClearSegments();
}

void ModMenuModule::SegmentSupport::AddSegment(SegmentBase* segment)
{
	auto it = std::find_if(m_segments.begin(), m_segments.end(), [segment](const SegmentEntry& entry) {
		return entry.segment == segment; 
	});
	if (it != m_segments.end()) {
		spdlog::error("SegmentSupport::AddSegment: Segment already added");
		return;
	}
	m_segments.push_back({ segment });
}

void ModMenuModule::SegmentSupport::AddAttachSetVisibleSegment(SegmentBase* segment, ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	AddSegment(segment);
	segment->SetAttached(true, menu, parent);
	if(segment->IsAttached() && m_visible) {
		segment->SetVisible(true);
	}
}

bool ModMenuModule::SegmentSupport::RemoveSegment(SegmentBase* segment)
{
	auto it = std::find_if(m_segments.begin(), m_segments.end(), [segment](const SegmentEntry& entry) {
		return entry.segment == segment; 
	});
	if (it == m_segments.end()) {
		spdlog::error("SegmentSupport::RemoveSegment: Segment not found");
		return false;
	}

	m_segments.erase(it);
	return true;
}

bool ModMenuModule::SegmentSupport::DeleteSegment(SegmentBase*& segment)
{
	if (!RemoveSegment(segment)) {
		return false;
	}

	segment->SetAttached(false);
	delete segment;
	segment = nullptr;
	return true;
}

void ModMenuModule::SegmentSupport::ClearSegments()
{
	for (SegmentEntry& entry : m_segments) {
		entry.segment->SetAttached(false);
		delete entry.segment;
	}
	m_segments.clear();
}

bool ModMenuModule::SegmentSupport::AttachSegment(SegmentBase* segment, ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	auto it = std::find_if(m_segments.begin(), m_segments.end(), [segment](const SegmentEntry& entry) {
		return entry.segment == segment;
	});
	if (it == m_segments.end()) {
		spdlog::error("SegmentSupport::AttachSegment: Segment is not managed by this SegmentSupport");
		return false;
	}

	segment->SetAttached(true, menu, parent);

	if (!segment->IsAttached()) {
		spdlog::error("SegmentSupport::AttachSegment: Segment failed to attach");
		return false;
	}

	if(m_visible) {
		segment->SetVisible(true);
	}

	return true;
}

bool ModMenuModule::SegmentSupport::DetachSegment(SegmentBase* segment)
{
	auto it = std::find_if(m_segments.begin(), m_segments.end(), [segment](const SegmentEntry& entry) {
		return entry.segment == segment;
	});
	if (it == m_segments.end()) {
		spdlog::error("SegmentSupport::DetachSegment: Segment is not managed by this SegmentSupport");
		return false;
	}

	segment->SetAttached(false);

	return true;
}

bool ModMenuModule::SegmentSupport::AttachAllSegments(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	for (SegmentEntry& entry : m_segments) {
		entry.segment->SetAttached(true, menu, parent);
		if (!entry.segment->IsAttached()) {
			spdlog::error("SegmentSupport::AttachAllSegments: Segment failed to attach");
			return false;
		}
		if (m_visible) {
			entry.segment->SetVisible(true);
		}
	}
	return true;
}

void ModMenuModule::SegmentSupport::DetachAllSegments()
{
	for (SegmentEntry& entry : m_segments) {
		entry.segment->SetAttached(false);
	}
}

void ModMenuModule::SegmentSupport::SetSegmentsVisible(bool visible)
{
	if (m_visible == visible) return;
	m_visible = visible;
	for (SegmentEntry& entry : m_segments) {
		if (!entry.segment->IsAttached()) continue;
		entry.segment->SetVisible(visible);
	}
}

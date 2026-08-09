#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../segment-support.h"

namespace ModMenuModule {
	class PedPreviewSegment;

	struct SpawnPedActionSegmentData {
		std::wstring pedTemplateName;
		bool inNearestCar;
	};

	class SpawnPedActionSegment : public Segment<SpawnPedActionSegmentData>, public SegmentSupport {
	public:
		SpawnPedActionSegment();
		virtual ~SpawnPedActionSegment();

		bool ValidateSegment();

		virtual std::optional<SpawnPedActionSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const SpawnPedActionSegmentData& data) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		void OnTemplateControllerSave(const std::wstring& templateName);
		void UpdatePreview();

		UiModule::SelectController<std::wstring>* m_templateNameController = nullptr;
		UiModule::SelectController<bool>* m_inNearestCarController = nullptr;
		PedPreviewSegment* m_pedPreviewSegment = nullptr;
	};
}

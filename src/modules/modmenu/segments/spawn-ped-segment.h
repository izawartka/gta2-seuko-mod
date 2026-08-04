#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../segment-support.h"
#include "../cheats/ped-templates.h"

namespace ModMenuModule {
	class PedPreviewSegment;

	struct SpawnPedSegmentData {
		PedTemplate pedTemplate;
		bool inNearestCar;
	};

	class SpawnPedSegment : public Segment<SpawnPedSegmentData>, public SegmentSupport {
	public:
		SpawnPedSegment();
		SpawnPedSegment(std::string_view persistencePrefix);
		virtual ~SpawnPedSegment();

		virtual std::optional<SpawnPedSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const SpawnPedSegmentData& data) override;

		bool OnPassedMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		void InitialCreateData();
		void OnTemplateControllerSave(const std::wstring& templateName);
		void UpdatePreview();

		std::string m_persistencePrefix = "";
		TempPedTemplateId m_tempTemplateId = -1;
		std::shared_ptr<PedTemplate> m_tempTemplatePtr = nullptr;
		UiModule::SelectController<std::wstring>* m_templateNameController = nullptr;
		UiModule::SelectController<bool>* m_inNearestCarController = nullptr;
		UiModule::MenuItemId m_customizeBtnMenuItemId = -1;
		PedPreviewSegment* m_pedPreviewSegment = nullptr;
	};
}

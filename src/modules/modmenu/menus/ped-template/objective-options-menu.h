#pragma once
#include "../../common.h"
#include "../../menu-base.h"
#include "../../segment-support.h"
#include "../../cheats/ped-templates.h"
#include "../../cheats/position-store.h"

namespace ModMenuModule {
	class PositionMenuSegment;
	class EncodedPedIdSegment;
	class EncodedCarIdSegment;
	class EncodedObjectIdSegment;

	class PedTemplateObjectiveOptionsMenu : public MenuBase, public SegmentSupport {
	public:
		PedTemplateObjectiveOptionsMenu(TempPedTemplateId tempTemplateId);
		virtual ~PedTemplateObjectiveOptionsMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		bool CheckSetupFirstAttach();
		void UpdateObjTargetItems();
		void SaveObjTargetPosition();

		TempPedTemplateId m_tempTemplateId = -1;
		std::shared_ptr<PedTemplate> m_tempTemplatePtr = nullptr;
		size_t m_objTargetItemsIndex = 0;
		bool m_isFirstAttach = true;
		UiModule::VertCont* m_objTargetItemsCont = nullptr;
		PositionStoreCheat::PositionId m_targetPosId = -1;
		PositionMenuSegment* m_targetPosSegment = nullptr;
		EncodedPedIdSegment* m_targetPedSegment = nullptr;
		EncodedCarIdSegment* m_targetCarSegment = nullptr;
		EncodedObjectIdSegment* m_targetObjectSegment = nullptr;
	};
}

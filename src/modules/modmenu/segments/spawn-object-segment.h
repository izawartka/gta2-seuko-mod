#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../utils/categorized-objects-utils.h"

namespace ModMenuModule {
	struct SpawnObjectSegmentData {
		Game::OBJECT_TYPE objectType;
	};

	class SpawnObjectSegment : public Segment<SpawnObjectSegmentData> {
	public:
		SpawnObjectSegment() = default;
		SpawnObjectSegment(const std::string& persistencePrefix);
		virtual ~SpawnObjectSegment();

		virtual std::optional<SpawnObjectSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const SpawnObjectSegmentData& data) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		void CreateObjectController(const std::vector<const Utils::CategorizedObjects::ObjectDef*>& objectOptionList, const Utils::CategorizedObjects::ObjectDef* selectedObject);
		void DestroyObjectController();
		void OnCategoryControllerSave(Utils::CategorizedObjects::ObjectCategory category);
		void CreateVariantController(const std::vector<Utils::CategorizedObjects::ObjectVariant>& variantOptionList, Utils::CategorizedObjects::ObjectVariant selectedVariant);
		void DestroyVariantController();
		void OnObjectControllerSave(const Utils::CategorizedObjects::ObjectDef* objectDef);

		UiModule::SelectController<Utils::CategorizedObjects::ObjectCategory>* m_categoryController = nullptr;
		UiModule::Text* m_objectText = nullptr;
		UiModule::MenuItemId m_objectMenuItemId = -1;
		UiModule::SelectController<const Utils::CategorizedObjects::ObjectDef*>* m_objectController = nullptr;
		UiModule::Text* m_variantText = nullptr;
		UiModule::MenuItemId m_variantMenuItemId = -1;
		UiModule::SelectController<Utils::CategorizedObjects::ObjectVariant>* m_variantController = nullptr;
		std::string m_persistencePrefix = "";
	};
}

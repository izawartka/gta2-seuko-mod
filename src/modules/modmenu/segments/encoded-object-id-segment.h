#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../utils/get-object-by-encoded-id.h"

namespace ModMenuModule {
	using EncodedObjectIdSegmentSaveCallback = std::function<void(Utils::EncodedObjectId)>;

	class EncodedObjectIdSegment : public SegmentBase {
	public:
		EncodedObjectIdSegment(const std::wstring& label, Utils::EncodedObjectId value);
		virtual ~EncodedObjectIdSegment();

		void SetSaveCallback(EncodedObjectIdSegmentSaveCallback callback);
		void SetValue(Utils::EncodedObjectId value);
		Utils::EncodedObjectId GetValue() const;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		static const std::wstring& GetTypeFromValue(Utils::EncodedObjectId value);
		static Utils::EncodedObjectId GetValueFromType(const std::wstring& type);

		void OnTypeControllerSave(const std::wstring& value);
		void OnCustomIdControllerSave(const int& value);
		void DestroyCustomIdController();
		void UpdateCustomIdController();

		std::wstring m_label;
		Utils::EncodedObjectId m_value;
		EncodedObjectIdSegmentSaveCallback m_saveCallback = nullptr;
		UiModule::SelectController<std::wstring>* m_typeController = nullptr;
		UiModule::MenuItemId m_typeMenuItemId = -1;
		UiModule::EditableController<int>* m_customIdController = nullptr;
		UiModule::MenuItemId m_customIdMenuItemId = -1;
	};
}

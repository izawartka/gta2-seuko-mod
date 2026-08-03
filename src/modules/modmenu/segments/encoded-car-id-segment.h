#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../utils/get-car-by-encoded-id.h"

namespace ModMenuModule {
	using EncodedCarIdSegmentSaveCallback = std::function<void(Utils::EncodedCarId)>;

	class EncodedCarIdSegment : public SegmentBase {
	public:
		EncodedCarIdSegment(const std::wstring& label, Utils::EncodedCarId value);
		virtual ~EncodedCarIdSegment();

		void SetSaveCallback(EncodedCarIdSegmentSaveCallback callback);
		void SetValue(Utils::EncodedCarId value);
		Utils::EncodedCarId GetValue() const;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		static const std::wstring& GetTypeFromValue(Utils::EncodedCarId value);
		static Utils::EncodedCarId GetValueFromType(const std::wstring& type);

		void OnTypeControllerSave(const std::wstring& value);
		void OnCustomIdControllerSave(const int& value);
		void DestroyCustomIdController();
		void UpdateCustomIdController();

		std::wstring m_label;
		Utils::EncodedCarId m_value;
		EncodedCarIdSegmentSaveCallback m_saveCallback = nullptr;
		UiModule::SelectController<std::wstring>* m_typeController = nullptr;
		UiModule::MenuItemId m_typeMenuItemId = -1;
		UiModule::EditableController<int>* m_customIdController = nullptr;
		UiModule::MenuItemId m_customIdMenuItemId = -1;
	};
}

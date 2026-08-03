#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../utils/get-ped-by-encoded-id.h"

namespace ModMenuModule {
	using EncodedPedIdSegmentSaveCallback = std::function<void(Utils::EncodedPedId)>;

	class EncodedPedIdSegment : public SegmentBase {
	public:
		EncodedPedIdSegment(const std::wstring& label, Utils::EncodedPedId value);
		virtual ~EncodedPedIdSegment();

		void SetSaveCallback(EncodedPedIdSegmentSaveCallback callback);
		void SetValue(Utils::EncodedPedId value);
		Utils::EncodedPedId GetValue() const;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		static const std::wstring& GetTypeFromValue(Utils::EncodedPedId value);
		static Utils::EncodedPedId GetValueFromType(const std::wstring& type);

		void OnTypeControllerSave(const std::wstring& value);
		void OnCustomIdControllerSave(const int& value);
		void DestroyCustomIdController();
		void UpdateCustomIdController();

		std::wstring m_label;
		Utils::EncodedPedId m_value;
		EncodedPedIdSegmentSaveCallback m_saveCallback = nullptr;
		UiModule::SelectController<std::wstring>* m_typeController = nullptr;
		UiModule::MenuItemId m_typeMenuItemId = -1;
		UiModule::EditableController<int>* m_customIdController = nullptr;
		UiModule::MenuItemId m_customIdMenuItemId = -1;
	};
}

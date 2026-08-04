#include "saveas-menu.h"
#include "../../root.h"
#include "../../../../converters/gta-font-safe.h"
#include "../../toast-manager.h"

ModMenuModule::PedTemplateSaveasMenu::PedTemplateSaveasMenu(TempPedTemplateId tempTemplateId)
{
	m_tempTemplateId = tempTemplateId;
}

ModMenuModule::PedTemplateSaveasMenu::~PedTemplateSaveasMenu()
{

}

bool ModMenuModule::PedTemplateSaveasMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Save character template#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	m_tempTemplatePtr = pedTemplatesCheat->GetTempTemplate(m_tempTemplateId);
	if (!m_tempTemplatePtr) {
		spdlog::error("PedTemplateSaveasMenu: Failed to get temp template with id {}", m_tempTemplateId);
		return false;
	}

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	// template name
	std::wstring templateName = m_tempTemplatePtr->name;
	auto* templateNameText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_templateNameController = m_menuController->CreateLatestItemController<UiModule::EditableController<std::wstring>>(
		templateNameText,
		templateName,
		UiModule::EditableControllerOptions{ L"Template name: #", L"#" }
	);
	m_templateNameController->SetConverter<GtaFontSafeConverter>();
	m_templateNameController->SetSaveCallback([this](const std::wstring& newValue) {
		m_overwriteConfirmed = false;
	});

	// save btn
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Save", options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PedTemplateSaveasMenu::Detach()
{
	m_overwriteConfirmed = false;
	DestroyMenu();
}

void ModMenuModule::PedTemplateSaveasMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	MenuManager* menuManager = MenuManager::GetInstance();

	switch (id) {
	case 0:
		menuManager->RemoveLastMenu();
		return;
	case 2:
		if (SaveTemplate()) {
			menuManager->RemoveLastMenu();
		}
		return;
	}
}

bool ModMenuModule::PedTemplateSaveasMenu::SaveTemplate()
{
	const std::wstring& templateName = m_templateNameController->GetValue().value();

	if (templateName.empty()) {
		ToastManager::GetInstance()->Show({ L"Template name cannot be empty", ToastType::Warning });
		return false;
	}

	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	const PedTemplate* existingTemplate = pedTemplatesCheat->GetTemplate(templateName);

	if (existingTemplate && existingTemplate->isBuiltin) {
		ToastManager::GetInstance()->Show({ L"Cannot overwrite built-in template", ToastType::Warning });
		return false;
	}

	if (existingTemplate && !m_overwriteConfirmed) {
		ToastManager::GetInstance()->Show({ L"Template already exists,", ToastType::Warning });
		ToastManager::GetInstance()->Show({ L"Press again to overwrite", ToastType::Warning });
		m_overwriteConfirmed = true;
		return false;
	}

	if (!pedTemplatesCheat->SaveTemplate(m_tempTemplateId, templateName)) {
		ToastManager::GetInstance()->Show({ L"Failed to save template", ToastType::Error });
		return false;
	}

	ToastManager::GetInstance()->Show({ L"Template saved" });
	return true;
}

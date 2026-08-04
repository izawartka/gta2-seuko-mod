#include "basic-options-menu.h"
#include "../../segments/ped-preview-segment.h"
#include "../../root.h"
#include "../../../../converters/ped-graphic-type.h"
#include "../../../../converters/ped-remap.h"
#include "../../../../converters/weapon.h"

ModMenuModule::PedTemplateBasicOptionsMenu::PedTemplateBasicOptionsMenu(TempPedTemplateId tempTemplateId)
{
	m_tempTemplateId = tempTemplateId;
	m_pedPreviewSegment = CreateSegment<PedPreviewSegment>();
}

ModMenuModule::PedTemplateBasicOptionsMenu::~PedTemplateBasicOptionsMenu()
{
	DeleteSegment(m_pedPreviewSegment);
}

bool ModMenuModule::PedTemplateBasicOptionsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Character template options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	m_tempTemplatePtr = pedTemplatesCheat->GetTempTemplate(m_tempTemplateId);
	if (!m_tempTemplatePtr) {
		spdlog::error("PedTemplateBasicOptionsMenu: Failed to get temp template with id {}", m_tempTemplateId);
		return false;
	}

	// graphic type
	const auto& graphicTypeOptions = Game::Utils::GetAvailablePedGraphicTypes();
	auto* graphicTypeText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* graphicTypeController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_GRAPHIC_TYPE>>(
		graphicTypeText,
		graphicTypeOptions,
		m_tempTemplatePtr->data.graphicType,
		UiModule::SelectControllerOptions{ L"Graphic type: #", L"#" }
	);
	graphicTypeController->SetSaveCallback([this](Game::PED_GRAPHIC_TYPE newValue) {
		m_tempTemplatePtr->data.graphicType = newValue;
		m_tempTemplatePtr->isSaved = false;
		UpdatePreview();
	});
	graphicTypeController->SetConverter<PedGraphicTypeConverter>();

	// remap
	const auto& remapOptions = Game::Utils::GetAvailablePedRemaps();
	auto* remapText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* remapController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_REMAP>>(
		remapText,
		remapOptions,
		m_tempTemplatePtr->data.remap,
		UiModule::SelectControllerOptions{ L"Remap: #", L"#" }
	);
	remapController->SetSaveCallback([this](Game::PED_REMAP newValue) {
		m_tempTemplatePtr->data.remap = newValue;
		m_tempTemplatePtr->isSaved = false;
		UpdatePreview();
	});
	remapController->SetConverter<PedRemapConverter>();

	// weapon
	const auto& weaponOptions = Game::Utils::GetAvailableWeaponsInclNoWeapon();
	auto* weaponText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* weaponController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::WEAPON_INDEX>>(
		weaponText,
		weaponOptions,
		m_tempTemplatePtr->data.weaponIndex,
		UiModule::SelectControllerOptions{ L"Weapon: #", L"#" }
	);
	weaponController->SetSaveCallback([this](Game::WEAPON_INDEX newValue) {
		m_tempTemplatePtr->data.weaponIndex = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	weaponController->SetConverter<WeaponConverter>();

	// health
	auto* healthText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* healthController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::ushort>>(
		healthText,
		m_tempTemplatePtr->data.health,
		UiModule::EditableControllerOptions{ L"Health: #", L"#" }
	);
	healthController->SetSaveCallback([this](Game::ushort newValue) {
		m_tempTemplatePtr->data.health = newValue;
		m_tempTemplatePtr->isSaved = false;
	});

	// ped preview
	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);
	m_pedPreviewSegment->SetPedData(m_tempTemplatePtr->data);
	AttachSegment(m_pedPreviewSegment, this, vertCont);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PedTemplateBasicOptionsMenu::Detach()
{
	DetachSegment(m_pedPreviewSegment);
	DestroyMenu();
}

void ModMenuModule::PedTemplateBasicOptionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: { // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	}
	default:
		break;
	}
}

void ModMenuModule::PedTemplateBasicOptionsMenu::UpdatePreview()
{
	m_pedPreviewSegment->SetPedData(m_tempTemplatePtr->data);
}

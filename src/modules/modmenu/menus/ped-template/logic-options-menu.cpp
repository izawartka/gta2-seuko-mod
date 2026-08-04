#include "logic-options-menu.h"
#include "../../segments/encoded-ped-id-segment.h"
#include "../../root.h"
#include "../../../../converters/ped-occupation.h"
#include "../../../../converters/ped-threat-search.h"
#include "../../../../converters/ped-threat-reaction.h"

ModMenuModule::PedTemplateLogicOptionsMenu::PedTemplateLogicOptionsMenu(TempPedTemplateId tempTemplateId)
{
	m_tempTemplateId = tempTemplateId;
	m_leaderIdSegment = CreateSegment<EncodedPedIdSegment>(L"Leader", -1);
}

ModMenuModule::PedTemplateLogicOptionsMenu::~PedTemplateLogicOptionsMenu()
{

}

bool ModMenuModule::PedTemplateLogicOptionsMenu::Attach()
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
		spdlog::error("PedTemplateLogicOptionsMenu: Failed to get temp template with id {}", m_tempTemplateId);
		return false;
	}

	// occupation
	const auto& occupationOptions = Game::Utils::GetAvailablePedOccupations();
	auto* occupationText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* occupationController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_OCCUPATION>>(
		occupationText,
		occupationOptions,
		m_tempTemplatePtr->data.occupation,
		UiModule::SelectControllerOptions{ L"Occupation: #", L"#" }
	);
	occupationController->SetSaveCallback([this](Game::PED_OCCUPATION newValue) {
		m_tempTemplatePtr->data.occupation = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	occupationController->SetConverter<PedOccupationConverter>();

	// threat search
	const auto& threatSearchOptions = Game::Utils::GetAvailablePedThreatSearches();
	auto* threatSearchText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* threatSearchController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_THREAT_SEARCH>>(
		threatSearchText,
		threatSearchOptions,
		m_tempTemplatePtr->data.threatSearch,
		UiModule::SelectControllerOptions{ L"Threat search: #", L"#" }
	);
	threatSearchController->SetSaveCallback([this](Game::PED_THREAT_SEARCH newValue) {
		m_tempTemplatePtr->data.threatSearch = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	threatSearchController->SetConverter<PedThreatSearchConverter>();

	// threat reaction
	const auto& threatReactionOptions = Game::Utils::GetAvailablePedThreatReactions();
	auto* threatReactionText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* threatReactionController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_THREAT_REACTION>>(
		threatReactionText,
		threatReactionOptions,
		m_tempTemplatePtr->data.threatReaction,
		UiModule::SelectControllerOptions{ L"Threat reaction: #", L"#" }
	);
	threatReactionController->SetSaveCallback([this](Game::PED_THREAT_REACTION newValue) {
		m_tempTemplatePtr->data.threatReaction = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	threatReactionController->SetConverter<PedThreatReactionConverter>();

	// leader
	m_leaderIdSegment->SetValue(m_tempTemplatePtr->data.leaderPedId);
	m_leaderIdSegment->SetSaveCallback([this](Utils::EncodedPedId newValue) {
		m_tempTemplatePtr->data.leaderPedId = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	AttachSegment(m_leaderIdSegment, this, vertCont);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PedTemplateLogicOptionsMenu::Detach()
{
	SaveCurrentSelectedIndex();
	DetachSegment(m_leaderIdSegment);
	DestroyMenu();
}

void ModMenuModule::PedTemplateLogicOptionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
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

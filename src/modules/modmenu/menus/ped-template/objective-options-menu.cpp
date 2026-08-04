#include "objective-options-menu.h"
#include "../../segments/position-menu-segment.h"
#include "../../segments/encoded-ped-id-segment.h"
#include "../../segments/encoded-car-id-segment.h"
#include "../../segments/encoded-object-id-segment.h"
#include "../../root.h"
#include "../../../../converters/ped-objective.h"

ModMenuModule::PedTemplateObjectiveOptionsMenu::PedTemplateObjectiveOptionsMenu(TempPedTemplateId tempTemplateId)
{
	m_tempTemplateId = tempTemplateId;
	m_targetPedSegment = CreateSegment<EncodedPedIdSegment>(L"Obj. target char.", -1);
	m_targetCarSegment = CreateSegment<EncodedCarIdSegment>(L"Obj. target vehicle", -1);
	m_targetObjectSegment = CreateSegment<EncodedObjectIdSegment>(L"Obj. target object", -1);
}

ModMenuModule::PedTemplateObjectiveOptionsMenu::~PedTemplateObjectiveOptionsMenu()
{
	SaveObjTargetPosition();
}

bool ModMenuModule::PedTemplateObjectiveOptionsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Character template options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	if (!CheckSetupFirstAttach()) {
		return false;
	}

	// objective
	const auto& objectiveOptions = Game::Utils::GetAvailablePedObjectives();
	auto* objectiveText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* objectiveController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_OBJECTIVE>>(
		objectiveText,
		objectiveOptions,
		m_tempTemplatePtr->data.objective,
		UiModule::SelectControllerOptions{ L"Objective: #", L"#" }
	);
	objectiveController->SetSaveCallback([this](Game::PED_OBJECTIVE newValue) {
		m_tempTemplatePtr->data.objective = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	objectiveController->SetConverter<PedObjectiveConverter>();

	// objective timer
	auto* objectiveTimerText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* objectiveTimerController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::ushort>>(
		objectiveTimerText,
		m_tempTemplatePtr->data.objectiveTimer,
		UiModule::EditableControllerOptions{ L"Objective timer: #", L"#" }
	);
	objectiveTimerController->SetClampCallback([](Game::ushort newValue) {
		return std::min(newValue, static_cast<unsigned short>(9999));
	});
	objectiveTimerController->SetSaveCallback([this](Game::ushort newValue) {
		m_tempTemplatePtr->data.objectiveTimer = newValue;
		m_tempTemplatePtr->isSaved = false;
	});

	// obj. target items
	m_objTargetItemsIndex = m_menuController->GetNextAddedItemIndex();
	m_objTargetItemsCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);
	UpdateObjTargetItems();

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PedTemplateObjectiveOptionsMenu::Detach()
{
	SaveCurrentSelectedIndex();
	DetachAllSegments();
	DestroyMenu();
}

void ModMenuModule::PedTemplateObjectiveOptionsMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::PedTemplateObjectiveOptionsMenu::OnHide()
{
	SetSegmentsVisible(false);
}

void ModMenuModule::PedTemplateObjectiveOptionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == 0) { // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		return;
	}

	if (m_targetPosSegment) {
		m_targetPosSegment->OnPassedMenuAction(item, id);
	}
}

bool ModMenuModule::PedTemplateObjectiveOptionsMenu::CheckSetupFirstAttach()
{
	if (!m_isFirstAttach) return true;

	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	m_tempTemplatePtr = pedTemplatesCheat->GetTempTemplate(m_tempTemplateId);
	if (!m_tempTemplatePtr) {
		spdlog::error("PedTemplateObjectiveOptionsMenu: Failed to get temp template with id {}", m_tempTemplateId);
		return false;
	}

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	m_targetPosId = positionStoreCheat->Create();
	positionStoreCheat->Update(m_targetPosId, m_tempTemplatePtr->data.objectiveTargets.position);
	m_targetPosSegment = CreateSegment<PositionMenuSegment>(m_targetPosId, "");

	m_isFirstAttach = false;
	return true;
}

void ModMenuModule::PedTemplateObjectiveOptionsMenu::UpdateObjTargetItems()
{
	m_menuController->SetNextAddedItemIndex(m_objTargetItemsIndex);

	AttachSegment(m_targetPosSegment, this, m_objTargetItemsCont);

	m_targetPedSegment->SetValue(m_tempTemplatePtr->data.objectiveTargets.pedId);
	m_targetPedSegment->SetSaveCallback([this](Utils::EncodedPedId newValue) {
		m_tempTemplatePtr->data.objectiveTargets.pedId = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	AttachSegment(m_targetPedSegment, this, m_objTargetItemsCont);

	m_targetCarSegment->SetValue(m_tempTemplatePtr->data.objectiveTargets.carId);
	m_targetCarSegment->SetSaveCallback([this](Utils::EncodedCarId newValue) {
		m_tempTemplatePtr->data.objectiveTargets.carId = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	AttachSegment(m_targetCarSegment, this, m_objTargetItemsCont);

	m_targetObjectSegment->SetValue(m_tempTemplatePtr->data.objectiveTargets.objectId);
	m_targetObjectSegment->SetSaveCallback([this](Utils::EncodedObjectId newValue) {
		m_tempTemplatePtr->data.objectiveTargets.objectId = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	AttachSegment(m_targetObjectSegment, this, m_objTargetItemsCont);
}

void ModMenuModule::PedTemplateObjectiveOptionsMenu::SaveObjTargetPosition()
{
	if (m_targetPosId == -1) {
		spdlog::error("PedTemplateObjectiveOptionsMenu: Failed to save target position, targetPosId is not set");
		return;
	}

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* storeEntry = positionStoreCheat->Get(m_targetPosId);
	if (!storeEntry) {
		spdlog::error("PedTemplateObjectiveOptionsMenu: Failed to save target position, storeEntry is nullptr");
		return;
	}

	if (!m_tempTemplatePtr) {
		spdlog::error("PedTemplateObjectiveOptionsMenu: Failed to save target position, tempTemplatePtr is nullptr");
		positionStoreCheat->Remove(m_targetPosId);
		return;
	}

	PositionStoreEntry* templateEntry = &m_tempTemplatePtr->data.objectiveTargets.position;
	if ((templateEntry->updateFromPlayerPed && storeEntry->updateFromPlayerPed) || (*templateEntry == *storeEntry)) {
		spdlog::debug("PedTemplateObjectiveOptionsMenu: Skipping target position save");
		positionStoreCheat->Remove(m_targetPosId);
		return;
	}

	spdlog::debug("PedTemplateObjectiveOptionsMenu: Saving target position");
	*templateEntry = *storeEntry;
	m_tempTemplatePtr->isSaved = false;
	positionStoreCheat->Remove(m_targetPosId);
}

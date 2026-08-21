#include "position-menu-segment.h"
#include "../../../converters/scr-vector3.h"
#include "../root.h"
#include "../cheats/position-store.h"
#include "../menus/position-menu.h"

ModMenuModule::PositionMenuSegment::PositionMenuSegment()
{
	m_ownsPositionId = true;

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	m_positionId = positionStoreCheat->Create();
}

ModMenuModule::PositionMenuSegment::PositionMenuSegment(std::string_view persistenceKey)
{
	m_persistenceKey = persistenceKey;
	m_ownsPositionId = true;

	const auto& loadedEntry = PositionStoreCheat::LoadFromPersistence(m_persistenceKey);
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	m_positionId = positionStoreCheat->Create(loadedEntry.has_value() ? *loadedEntry : PositionStoreEntry{});
}

ModMenuModule::PositionMenuSegment::PositionMenuSegment(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey)
{
	m_positionId = positionId;
	m_persistenceKey = persistenceKey;
	m_ownsPositionId = false;
}

ModMenuModule::PositionMenuSegment::~PositionMenuSegment()
{
	if (m_ownsPositionId) {
		PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
		positionStoreCheat->Remove(m_positionId);
	}
}

std::optional<ModMenuModule::PositionMenuSegmentData> ModMenuModule::PositionMenuSegment::GetSegmentData() const
{
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);

	if (!entry) {
		spdlog::error("PositionSegment: Cannot get segment data: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		return std::nullopt;
	}

	return PositionMenuSegmentData{
		entry->updateFromPlayerPed,
		entry->value.position,
		entry->autoZ,
	};
}

bool ModMenuModule::PositionMenuSegment::SetSegmentData(const PositionMenuSegmentData& data)
{
	PositionStoreEntry newEntry = {
		data.position,
		0,
		data.updateFromPlayer,
		data.autoZ
	};

	PositionStoreCheat::ApplyNow(newEntry);

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	bool success = positionStoreCheat->Update(m_positionId, newEntry);

	if (!success) {
		spdlog::error("PositionSegment: Failed to update position ID {} in PositionStoreCheat.", m_positionId);
		return false;
	}

	if (IsAttached()) UpdateTexts();
	return true;
}

void ModMenuModule::PositionMenuSegment::SetLabel(std::wstring_view label)
{
	m_label = label;
	if (IsAttached()) UpdateTexts();
}

const std::wstring& ModMenuModule::PositionMenuSegment::GetLabel() const
{
	return m_label;
}

bool ModMenuModule::PositionMenuSegment::OnPassedMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == m_positionContMenuItemId) {
		MenuManager::GetInstance()->AddMenu<PositionMenu>(m_positionId, m_persistenceKey);
		return true;
	}

	return false;
}

bool ModMenuModule::PositionMenuSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);
	if (!entry) {
		spdlog::error("PositionSegment: Cannot attach PositionMenuSegment: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		return false;
	}

	// position texts
	m_positionCont = m_menuController->CreateItem<UiModule::VertCont>(m_vertCont);
	m_positionContMenuItemId = m_menuController->GetLatestMenuItemId();
	m_modeText = uiRoot->AddComponent<UiModule::Text>(m_positionCont, L"", options.textSize);
	m_valueText = uiRoot->AddComponent<UiModule::Text>(m_positionCont, L"", options.textSize);
	UpdateTexts();

	return true;
}

void ModMenuModule::PositionMenuSegment::Detach()
{
	m_modeText = nullptr;
	m_valueText = nullptr;
	DestroySegment();
}

void ModMenuModule::PositionMenuSegment::OnShow()
{
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);
	SetEventListener<PositionStoreEntriesUpdateEvent>(&PositionMenuSegment::OnPositionStoreEntriesUpdate, entry && entry->updateFromPlayerPed);
	UpdateTexts();
}

void ModMenuModule::PositionMenuSegment::OnHide()
{
	RemoveEventListener<PositionStoreEntriesUpdateEvent>(true);
}

void ModMenuModule::PositionMenuSegment::OnPositionStoreEntriesUpdate(ModMenuModule::PositionStoreEntriesUpdateEvent& event)
{
	UpdateTexts();
}

bool ModMenuModule::PositionMenuSegment::UpdateTexts()
{
	if (!m_modeText || !m_valueText) {
		spdlog::error("PositionMenuSegment: Cannot update texts: texts are not initialized.");
		return false;
	}

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);

	if (!entry) {
		spdlog::error("PositionMenuSegment: Cannot update text: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		return false;
	}

	std::wstring modeTextValue = (entry->updateFromPlayerPed) ? L"#From player#" : L"#Custom#";
	std::wstring modeText = m_label + L": " + modeTextValue;
	m_modeText->SetText(modeText);

	std::wstring valueText = L"#" + ScrVector3Converter::ConvertToString(entry->value.position) + L"#";
	m_valueText->SetText(valueText);

	return true;
}

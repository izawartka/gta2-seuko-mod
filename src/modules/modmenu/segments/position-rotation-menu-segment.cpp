#include "position-rotation-menu-segment.h"
#include "../../../converters/scr-vector3-rot.h"
#include "../root.h"
#include "../cheats/position-store.h"
#include "../menus/position-rotation-menu.h"

ModMenuModule::PositionRotationMenuSegment::PositionRotationMenuSegment()
{
	m_ownsPositionId = true;

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	m_positionId = positionStoreCheat->Create();
}

ModMenuModule::PositionRotationMenuSegment::PositionRotationMenuSegment(std::string_view persistenceKey)
{
	m_persistenceKey = persistenceKey;
	m_ownsPositionId = true;

	const auto& loadedEntry = PositionStoreCheat::LoadFromPersistence(m_persistenceKey);
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	m_positionId = positionStoreCheat->Create(loadedEntry.has_value() ? *loadedEntry : PositionStoreEntry{});
}

ModMenuModule::PositionRotationMenuSegment::PositionRotationMenuSegment(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey)
{
	m_positionId = positionId;
	m_persistenceKey = persistenceKey;
	m_ownsPositionId = false;
}

ModMenuModule::PositionRotationMenuSegment::~PositionRotationMenuSegment()
{
	if (m_ownsPositionId) {
		PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
		positionStoreCheat->Remove(m_positionId);
	}
}

std::optional<ModMenuModule::PositionRotationMenuSegmentData> ModMenuModule::PositionRotationMenuSegment::GetSegmentData() const
{
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);

	if (!entry) {
		spdlog::error("PositionRotationSegment: Cannot get segment data: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		return std::nullopt;
	}

	return PositionRotationMenuSegmentData{
		entry->updateFromPlayerPed,
		entry->value.position,
		entry->autoZ,
		entry->value.rotation
	};
}

bool ModMenuModule::PositionRotationMenuSegment::SetSegmentData(const PositionRotationMenuSegmentData& data)
{
	PositionStoreEntry newEntry = {
		data.position,
		data.rotation,
		data.updateFromPlayer,
		data.autoZ
	};

	PositionStoreCheat::ApplyNow(newEntry);

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	bool success = positionStoreCheat->Update(m_positionId, newEntry);

	if (!success) {
		spdlog::error("PositionRotationSegment: Failed to update position ID {} in PositionStoreCheat.", m_positionId);
		return false;
	}

	if (IsAttached()) UpdateTexts();
	return true;
}

void ModMenuModule::PositionRotationMenuSegment::SetLabel(std::wstring_view label)
{
	m_label = label;
	if (IsAttached()) UpdateTexts();
}

const std::wstring& ModMenuModule::PositionRotationMenuSegment::GetLabel() const
{
	return m_label;
}

bool ModMenuModule::PositionRotationMenuSegment::OnPassedMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == m_positionContMenuItemId) {
		MenuManager::GetInstance()->AddMenu<PositionRotationMenu>(m_positionId, m_persistenceKey);
		return true;
	}

	return false;
}

bool ModMenuModule::PositionRotationMenuSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);
	if (!entry) {
		spdlog::error("PositionRotationSegment: Cannot attach PositionRotationMenuSegment: position ID {} does not exist in PositionStoreCheat.", m_positionId);
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

void ModMenuModule::PositionRotationMenuSegment::Detach()
{
	m_modeText = nullptr;
	m_valueText = nullptr;
	DestroySegment();
}

void ModMenuModule::PositionRotationMenuSegment::OnShow()
{
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);
	SetEventListener<PositionStoreEntriesUpdateEvent>(&PositionRotationMenuSegment::OnPositionStoreEntriesUpdate, entry && entry->updateFromPlayerPed);
	UpdateTexts();
}

void ModMenuModule::PositionRotationMenuSegment::OnHide()
{
	RemoveEventListener<PositionStoreEntriesUpdateEvent>(true);
}

void ModMenuModule::PositionRotationMenuSegment::OnPositionStoreEntriesUpdate(ModMenuModule::PositionStoreEntriesUpdateEvent& event)
{
	UpdateTexts();
}

bool ModMenuModule::PositionRotationMenuSegment::UpdateTexts()
{
	if (!m_modeText || !m_valueText) {
		spdlog::error("PositionRotationMenuSegment: Cannot update texts: texts are not initialized.");
		return false;
	}

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);

	if (!entry) {
		spdlog::error("PositionRotationMenuSegment: Cannot update text: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		return false;
	}

	std::wstring modeTextValue = (entry->updateFromPlayerPed) ? L"From player" : L"Custom";
	std::wstring modeText = m_label + L": " + modeTextValue;
	m_modeText->SetText(modeText);

	std::wstring valueText = L"#" + ScrVector3RotConverter::ConvertToString({ entry->value.position, entry->value.rotation }) + L"#";
	m_valueText->SetText(valueText);

	return true;
}

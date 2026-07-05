#include "position-segment.h"
#include "../../../converters/yes-no.h"
#include "../../../converters/scrf.h"
#include "../../../converters/enabled-disabled.h"
#include "../root.h"

ModMenuModule::PositionSegment::PositionSegment()
{
	m_ownsPositionId = true;

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	m_positionId = positionStoreCheat->Create();
}

ModMenuModule::PositionSegment::PositionSegment(std::string_view persistenceKey)
{
	m_persistenceKey = persistenceKey;
	m_ownsPositionId = true;

	const auto& loadedEntry = PositionStoreCheat::LoadFromPersistence(m_persistenceKey);
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	m_positionId = positionStoreCheat->Create(loadedEntry.has_value() ? *loadedEntry : PositionStoreEntry{});
}

ModMenuModule::PositionSegment::PositionSegment(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey)
{
	m_positionId = positionId;
	m_persistenceKey = persistenceKey;
	m_ownsPositionId = false;
}

ModMenuModule::PositionSegment::~PositionSegment()
{
	if (m_ownsPositionId) {
		PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
		positionStoreCheat->Remove(m_positionId);
	}
}

std::optional<ModMenuModule::PositionSegmentData> ModMenuModule::PositionSegment::GetSegmentData() const
{
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);

	if (!entry) {
		spdlog::error("PositionSegment: Cannot get segment data: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		return std::nullopt;
	}

	return PositionSegmentData{
		entry->updateFromPlayerPed,
		entry->value.position,
		entry->autoZ
	};
}

bool ModMenuModule::PositionSegment::SetSegmentData(const PositionSegmentData& data)
{
	PositionStoreEntry newEntry = {
		data.position,
		0, // rotation is not used
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

	UpdateControllers();
	return true;
}

bool ModMenuModule::PositionSegment::SetDoUpdatePosition(bool doUpdateFromPlayer)
{
	return OnDoUpdatePositionControllerSave(doUpdateFromPlayer);
}

bool ModMenuModule::PositionSegment::GetDoUpdatePosition() const
{
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);

	return entry && entry->updateFromPlayerPed;
}

bool ModMenuModule::PositionSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);
	if (!entry) {
		spdlog::error("PositionSegment: Cannot attach PositionSegment: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		return false;
	}

	// Update position
	UiModule::Text* doUpdatePositionText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_doUpdatePositionController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		doUpdatePositionText,
		std::vector<bool>{ false, true },
		entry->updateFromPlayerPed,
		UiModule::SelectControllerOptions{ L"Update position: #", L"#" }
	);
	m_doUpdatePositionController->SetConverter<YesNoConverter>();
	m_doUpdatePositionController->SetSaveCallback(std::bind(&PositionSegment::OnDoUpdatePositionControllerSave, this, std::placeholders::_1));

	// X position
	UiModule::Text* xText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_xController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::SCR_f>>(
		xText,
		entry->value.position.x,
		UiModule::EditableControllerOptions{ L"X: #", L"#" }
	);
	m_xController->SetConverter<ScrfConverter>();
	m_xController->SetSaveCallback(std::bind(&PositionSegment::OnCoordControllerSave, this, 0, std::placeholders::_1));
	m_xController->SetClampCallback(std::bind(&PositionSegment::ClampCoord, std::placeholders::_1, false));

	// Y position
	UiModule::Text* yText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_yController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::SCR_f>>(
		yText,
		entry->value.position.y,
		UiModule::EditableControllerOptions{ L"Y: #", L"#" }
	);
	m_yController->SetConverter<ScrfConverter>();
	m_yController->SetSaveCallback(std::bind(&PositionSegment::OnCoordControllerSave, this, 1, std::placeholders::_1));
	m_yController->SetClampCallback(std::bind(&PositionSegment::ClampCoord, std::placeholders::_1, false));

	// Z position
	UiModule::Text* zText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_zController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::SCR_f>>(
		zText,
		entry->value.position.z,
		UiModule::EditableControllerOptions{ L"Z: #", L"#" }
	);
	m_zController->SetConverter<ScrfConverter>();
	m_zController->SetSaveCallback(std::bind(&PositionSegment::OnCoordControllerSave, this, 2, std::placeholders::_1));
	m_zController->SetClampCallback(std::bind(&PositionSegment::ClampCoord, std::placeholders::_1, true));

	// Auto Z
	UiModule::Text* autoZText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_autoZController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		autoZText,
		std::vector<bool>{ false, true },
		entry->autoZ,
		UiModule::SelectControllerOptions{ L"Auto Z: #", L"#" }
	);
	m_autoZController->SetConverter<EnabledDisabledConverter>();
	m_autoZController->SetSaveCallback(std::bind(&PositionSegment::OnAutoZControllerSave, this, std::placeholders::_1));

	return true;
}

void ModMenuModule::PositionSegment::Detach()
{
	if (m_persistenceKey.size()) {
		PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
		const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);
		if (entry) {
			PositionStoreCheat::SaveToPersistence(m_persistenceKey, *entry);
		}
		else {
			spdlog::error("PositionSegment: Cannot save PositionSegment data to persistence: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		}
	}

	DestroySegment();
}

void ModMenuModule::PositionSegment::OnShow()
{
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);
	SetEventListener<PositionStoreEntriesUpdateEvent>(&PositionSegment::OnPositionStoreEntriesUpdate, entry && entry->updateFromPlayerPed);
	UpdateControllers();
}

void ModMenuModule::PositionSegment::OnHide()
{
	RemoveEventListener<PositionStoreEntriesUpdateEvent>(true);
}

void ModMenuModule::PositionSegment::OnPositionStoreEntriesUpdate(ModMenuModule::PositionStoreEntriesUpdateEvent& event)
{
	UpdateControllers();
}

Game::SCR_f ModMenuModule::PositionSegment::ClampCoord(Game::SCR_f value, bool isZCoord)
{
	return isZCoord ?
		Game::Utils::ClampZCoordToSafe(value) :
		Game::Utils::ClampCoordToSafe(value);
}

bool ModMenuModule::PositionSegment::UpdateControllers()
{
	if (!m_xController || !m_yController || !m_zController || !m_autoZController) {
		spdlog::error("PositionSegment: Cannot update controllers: controllers are not initialized.");
		return false;
	}

	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);

	if (!entry) {
		spdlog::error("PositionSegment: Cannot update controllers: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		return false;
	}

	m_doUpdatePositionController->SetValue(entry->updateFromPlayerPed);
	m_xController->SetValue(entry->value.position.x);
	m_yController->SetValue(entry->value.position.y);
	m_zController->SetValue(entry->value.position.z);
	m_autoZController->SetValue(entry->autoZ);

	if (IsVisible()) {
		SetEventListener<PositionStoreEntriesUpdateEvent>(&PositionSegment::OnPositionStoreEntriesUpdate, entry->updateFromPlayerPed);
	}

	return true;
}

bool ModMenuModule::PositionSegment::OnDoUpdatePositionControllerSave(bool newValue)
{
	if (!UpdateEntry([newValue](PositionStoreEntry& entry) {
		if (entry.updateFromPlayerPed == newValue) return false;
		entry.updateFromPlayerPed = newValue;
		return true;
	})) {
		spdlog::error("PositionSegment: Failed to set doUpdateFromPlayer for position ID {} in PositionStoreCheat.", m_positionId);
		return false;
	}

	if (IsVisible()) {
		SetEventListener<PositionStoreEntriesUpdateEvent>(&PositionSegment::OnPositionStoreEntriesUpdate, newValue);
	}

	if (IsAttached()) {
		UpdateControllers();
	}

	return true;
}

void ModMenuModule::PositionSegment::OnCoordControllerSave(size_t coordIndex, Game::SCR_f newValue)
{
	if (!UpdateEntry([m_positionId = m_positionId, coordIndex, newValue](PositionStoreEntry& entry) {
		switch (coordIndex) {
		case 0:
			if (ScrfConverter::AreEqual(entry.value.position.x, newValue)) return false;
			entry.value.position.x = newValue;
			break;
		case 1:
			if (ScrfConverter::AreEqual(entry.value.position.y, newValue)) return false;
			entry.value.position.y = newValue;
			break;
		case 2:
			if (ScrfConverter::AreEqual(entry.value.position.z, newValue)) return false;
			entry.value.position.z = newValue;
			entry.autoZ = false; // Disable auto Z if user manually sets Z
			break;
		default:
			spdlog::error("PositionSegment: Invalid coordinate index {} for position ID {} in PositionStoreCheat.", coordIndex, m_positionId);
			return false;
		}

		entry.updateFromPlayerPed = false; // Disable update from player if user manually sets a coordinate
		return true;
	})) {
		spdlog::error("PositionSegment: Failed to update coordinate index {} for position ID {} in PositionStoreCheat.", coordIndex, m_positionId);
	}

	UpdateControllers();
}

void ModMenuModule::PositionSegment::OnAutoZControllerSave(bool newValue)
{
	if (!UpdateEntry([newValue](PositionStoreEntry& entry) {
		if (entry.autoZ == newValue) return false;
		entry.autoZ = newValue;
		return true;
	})) {
		spdlog::error("PositionSegment: Failed to set autoZ for position ID {} in PositionStoreCheat.", m_positionId);
	}

	UpdateControllers();
}

bool ModMenuModule::PositionSegment::UpdateEntry(std::function<bool(PositionStoreEntry&)> updateFunc)
{
	PositionStoreCheat* positionStoreCheat = PositionStoreCheat::GetInstance();
	const PositionStoreEntry* entry = positionStoreCheat->Get(m_positionId);
	if (!entry) {
		spdlog::error("PositionSegment: Cannot update entry: position ID {} does not exist in PositionStoreCheat.", m_positionId);
		return false;
	}

	PositionStoreEntry newEntry = *entry;

	if (!updateFunc(newEntry)) {
		return true;
	}

	return positionStoreCheat->Update(m_positionId, newEntry);
}

#include "position-segment.h"
#include "../../../converters/scrf.h"
#include "../../../converters/enabled-disabled.h"
#include "../cheats/player-pos.h"
#include "../root.h"

ModMenuModule::PositionSegment::PositionSegment(const std::string& persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::PositionSegment::~PositionSegment()
{

}

std::optional<ModMenuModule::PositionSegmentData> ModMenuModule::PositionSegment::GetSegmentData() const
{
	if (!m_xController || !m_yController || !m_zController || !m_autoZController) {
		spdlog::error("Cannot get segment data: controllers are not initialized.");
		return std::nullopt;
	}

	const auto& positionOpt = GetCoordControllerValues();
	const auto& autoZOpt = m_autoZController->GetValue();

	if (!positionOpt.has_value() || !autoZOpt.has_value()) {
		spdlog::error("Cannot get segment data: controllers have invalid values.");
		return std::nullopt;
	}

	return PositionSegmentData{
		positionOpt.value(),
		autoZOpt.value()
	};
}

bool ModMenuModule::PositionSegment::SetSegmentData(const PositionSegmentData& data)
{
	if (!m_xController || !m_yController || !m_zController || !m_autoZController) {
		spdlog::error("Cannot set segment data: controllers are not initialized.");
		return false;
	}

	SetCoordControllerValues(data.position);
	m_autoZController->SetValue(data.autoZ);
	return true;
}

void ModMenuModule::PositionSegment::SetDoUpdateFromPlayer(bool doUpdateFromPlayer)
{
	if (m_doUpdateFromPlayer == doUpdateFromPlayer) return;
	m_doUpdateFromPlayer = doUpdateFromPlayer;

	if (!IsVisible()) return;

	if (doUpdateFromPlayer) {
		AddEventListener<ModMenuModule::PlayerPosUpdateEvent>(&PositionSegment::OnPlayerPosUpdate);
		ForceUpdateFromPlayer();
	}
	else {
		RemoveEventListener<ModMenuModule::PlayerPosUpdateEvent>();
	}
}

bool ModMenuModule::PositionSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	Game::SCR_f selectedX = Game::Utils::FromFloat(0.0f);
	Game::SCR_f selectedY = Game::Utils::FromFloat(0.0f);
	Game::SCR_f selectedZ = Game::Utils::FromFloat(0.0f);
	bool selectedAutoZ = true;

	if (m_persistencePrefix.size()) {
		selectedX = persistence->Load(m_persistencePrefix + "_SelectedX", Game::Utils::FromFloat(0.0f));
		selectedY = persistence->Load(m_persistencePrefix + "_SelectedY", Game::Utils::FromFloat(0.0f));
		selectedZ = persistence->Load(m_persistencePrefix + "_SelectedZ", Game::Utils::FromFloat(0.0f));
		selectedAutoZ = persistence->Load(m_persistencePrefix + "_SelectedAutoZ", true);
	}

	// X position
	UiModule::Text* xText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_xController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::SCR_f>>(
		xText,
		selectedX,
		UiModule::EditableControllerOptions{ L"X: #", L"#" }
	);
	m_xController->SetConverter<ScrfConverter>();
	m_xController->SetSaveCallback(std::bind(&PositionSegment::OnCoordControllerSave, this, false));

	// Y position
	UiModule::Text* yText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_yController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::SCR_f>>(
		yText,
		selectedY,
		UiModule::EditableControllerOptions{ L"Y: #", L"#" }
	);
	m_yController->SetConverter<ScrfConverter>();
	m_yController->SetSaveCallback(std::bind(&PositionSegment::OnCoordControllerSave, this, false));

	// Z position
	UiModule::Text* zText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_zController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::SCR_f>>(
		zText,
		selectedZ,
		UiModule::EditableControllerOptions{ L"Z: #", L"#" }
	);
	m_zController->SetConverter<ScrfConverter>();
	m_zController->SetSaveCallback(std::bind(&PositionSegment::OnCoordControllerSave, this, true));

	// Auto Z
	UiModule::Text* autoZText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_autoZController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		autoZText,
		std::vector<bool>{ false, true },
		selectedAutoZ,
		UiModule::SelectControllerOptions{ L"Auto Z: #", L"#" }
	);
	m_autoZController->SetConverter<EnabledDisabledConverter>();
	m_autoZController->SetSaveCallback(std::bind(&PositionSegment::OnAutoZControllerSave, this, std::placeholders::_1));

	return true;
}

void ModMenuModule::PositionSegment::Detach()
{
	if (m_persistencePrefix.size()) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
		if (m_xController && m_xController->GetValue().has_value()) {
			persistence->Save(m_persistencePrefix + "_SelectedX", m_xController->GetValue().value());
		}
		if (m_yController && m_yController->GetValue().has_value()) {
			persistence->Save(m_persistencePrefix + "_SelectedY", m_yController->GetValue().value());
		}
		if (m_zController && m_zController->GetValue().has_value()) {
			persistence->Save(m_persistencePrefix + "_SelectedZ", m_zController->GetValue().value());
		}
		if (m_autoZController && m_autoZController->GetValue().has_value()) {
			persistence->Save(m_persistencePrefix + "_SelectedAutoZ", m_autoZController->GetValue().value());
		}
	}

	DestroySegment();
}

void ModMenuModule::PositionSegment::OnShow()
{
	SetEventListener<ModMenuModule::PlayerPosUpdateEvent>(&PositionSegment::OnPlayerPosUpdate, m_doUpdateFromPlayer);
	if (m_doUpdateFromPlayer) ForceUpdateFromPlayer();
}

void ModMenuModule::PositionSegment::OnHide()
{
	RemoveEventListener<ModMenuModule::PlayerPosUpdateEvent>();
}

void ModMenuModule::PositionSegment::SetCoordControllerValues(const Game::SCR_Vector3& position)
{
	if (!m_xController || !m_yController || !m_zController) return;

	m_xController->SetValue(position.x);
	m_yController->SetValue(position.y);
	m_zController->SetValue(position.z);
}

std::optional<Game::SCR_Vector3> ModMenuModule::PositionSegment::GetCoordControllerValues() const
{
	if (!m_xController || !m_yController || !m_zController) return std::nullopt;

	const auto& xOpt = m_xController->GetValue();
	const auto& yOpt = m_yController->GetValue();
	const auto& zOpt = m_zController->GetValue();

	if (!xOpt.has_value() || !yOpt.has_value() || !zOpt.has_value()) {
		return std::nullopt;
	}

	return Game::SCR_Vector3{
		xOpt.value(),
		yOpt.value(),
		zOpt.value()
	};
}

void ModMenuModule::PositionSegment::OnPlayerPosUpdate(ModMenuModule::PlayerPosUpdateEvent& event)
{
	if (!m_doUpdateFromPlayer) {
		return;
	}

	const auto& position = event.GetPosition();
	if (!position.has_value()) {
		return;
	}

	SetCoordControllerValues(position.value());
}

void ModMenuModule::PositionSegment::ApplyAutoZIfNeeded()
{
	if (!m_autoZController) return;
	if (!m_autoZController->GetValue().value_or(false)) return;

	const auto& position = GetCoordControllerValues();
	if (!position.has_value()) {
		spdlog::warn("Cannot apply Auto Z: position controllers have invalid values");
		return;
	}

	Game::MapBlocks* mapBlocks = Game::Memory::GetMapBlocks();
	if (!mapBlocks) {
		spdlog::warn("Cannot apply Auto Z: MapBlocks is null");
		return;
	}

	Game::SCR_f z = 0;
	Game::Functions::FindMaxZ(mapBlocks, 0, &z, position->x, position->y);

	if (z == 0) {
		spdlog::warn("Cannot apply Auto Z: FindMaxZ returned Z=0 for position ({}, {})", Game::Utils::ToFloat(position->x), Game::Utils::ToFloat(position->y));
		return;
	}

	m_zController->SetValue(z);
}

void ModMenuModule::PositionSegment::ForceUpdateFromPlayer()
{
	PlayerPosCheat* playerPosCheat = GetCheat<PlayerPosCheat>();
	if (!playerPosCheat || !playerPosCheat->IsEnabled()) return;
	const auto& position = playerPosCheat->GetLastPosition();
	if (!position.has_value()) return;
	SetCoordControllerValues(position.value());
}

void ModMenuModule::PositionSegment::OnCoordControllerSave(bool isZCoord)
{
	SetDoUpdateFromPlayer(false);

	if (isZCoord) {
		m_autoZController->SetValue(false);
	}
	else {
		ApplyAutoZIfNeeded();
	}
}

void ModMenuModule::PositionSegment::OnAutoZControllerSave(bool newValue)
{
	if (newValue) ApplyAutoZIfNeeded();
}

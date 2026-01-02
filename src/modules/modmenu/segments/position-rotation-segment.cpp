#include "position-rotation-segment.h"
#include "../../../converters/yes-no.h"
#include "../../../converters/scrf.h"
#include "../../../converters/enabled-disabled.h"
#include "../../../converters/gta-angle.h"
#include "../cheats/player-pos.h"
#include "../root.h"
#include "position-rotation-segment.h"

ModMenuModule::PositionRotationSegment::PositionRotationSegment(const std::string& persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::PositionRotationSegment::~PositionRotationSegment()
{

}

std::optional<ModMenuModule::PositionRotationSegmentData> ModMenuModule::PositionRotationSegment::GetSegmentData() const
{
	if (!m_xController || !m_yController || !m_zController || !m_autoZController || !m_rotationController) {
		spdlog::error("Cannot get segment data: controllers are not initialized.");
		return std::nullopt;
	}

	const auto& positionOpt = GetCoordControllerValues();
	const auto& autoZOpt = m_autoZController->GetValue();
	const auto& rotationOpt = m_rotationController->GetValue();

	if (!positionOpt.has_value() || !autoZOpt.has_value() || !rotationOpt.has_value()) {
		spdlog::error("Cannot get segment data: controllers have invalid values.");
		return std::nullopt;
	}

	return PositionRotationSegmentData{
		m_doUpdatePosition,
		positionOpt.value(),
		autoZOpt.value(),
		rotationOpt.value()
	};
}

bool ModMenuModule::PositionRotationSegment::SetSegmentData(const PositionRotationSegmentData& data)
{
	if (!m_xController || !m_yController || !m_zController || !m_autoZController || !m_rotationController) {
		spdlog::error("Cannot set segment data: controllers are not initialized.");
		return false;
	}

	SetDoUpdatePosition(data.updateFromPlayer);
	if (!m_doUpdatePosition) {
		SetCoordControllerValues(data.position);
		m_rotationController->SetValue(data.rotation);
	}
	m_autoZController->SetValue(data.autoZ);
	return true;
}

void ModMenuModule::PositionRotationSegment::SetDoUpdatePosition(bool doUpdateFromPlayer)
{
	OnDoUpdatePositionControllerSave(doUpdateFromPlayer);

	if (!IsAttached()) return;
	m_doUpdatePositionController->SetValue(doUpdateFromPlayer);
}

bool ModMenuModule::PositionRotationSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	Game::SCR_f selectedX = Game::Utils::FromFloat(0.0f);
	Game::SCR_f selectedY = Game::Utils::FromFloat(0.0f);
	Game::SCR_f selectedZ = Game::Utils::FromFloat(0.0f);
	bool selectedAutoZ = true;
	short selectedRotation = 0;

	if (m_persistencePrefix.size()) {
		m_doUpdatePosition = persistence->Load(m_persistencePrefix + "_SelectedDoUpdatePosition", true);
		selectedX = persistence->Load(m_persistencePrefix + "_SelectedX", Game::Utils::FromFloat(0.0f));
		selectedY = persistence->Load(m_persistencePrefix + "_SelectedY", Game::Utils::FromFloat(0.0f));
		selectedZ = persistence->Load(m_persistencePrefix + "_SelectedZ", Game::Utils::FromFloat(0.0f));
		selectedAutoZ = persistence->Load(m_persistencePrefix + "_SelectedAutoZ", true);
		selectedRotation = persistence->Load(m_persistencePrefix + "_SelectedRotation", static_cast<short>(0));
	}

	// Update position
	UiModule::Text* doUpdatePositionText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_doUpdatePositionController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		doUpdatePositionText,
		std::vector<bool>{ false, true },
		m_doUpdatePosition,
		UiModule::SelectControllerOptions{ L"Update position: #", L"#" }
	);
	m_doUpdatePositionController->SetConverter<YesNoConverter>();
	m_doUpdatePositionController->SetSaveCallback(std::bind(&PositionRotationSegment::OnDoUpdatePositionControllerSave, this, std::placeholders::_1));

	// X position
	UiModule::Text* xText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_xController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::SCR_f>>(
		xText,
		selectedX,
		UiModule::EditableControllerOptions{ L"X: #", L"#" }
	);
	m_xController->SetConverter<ScrfConverter>();
	m_xController->SetSaveCallback(std::bind(&PositionRotationSegment::OnCoordControllerSave, this, false));
	m_xController->SetValidateCallback(std::bind(&PositionRotationSegment::ValidateCoord, std::placeholders::_1, false));

	// Y position
	UiModule::Text* yText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_yController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::SCR_f>>(
		yText,
		selectedY,
		UiModule::EditableControllerOptions{ L"Y: #", L"#" }
	);
	m_yController->SetConverter<ScrfConverter>();
	m_yController->SetSaveCallback(std::bind(&PositionRotationSegment::OnCoordControllerSave, this, false));
	m_yController->SetValidateCallback(std::bind(&PositionRotationSegment::ValidateCoord, std::placeholders::_1, false));

	// Z position
	UiModule::Text* zText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_zController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::SCR_f>>(
		zText,
		selectedZ,
		UiModule::EditableControllerOptions{ L"Z: #", L"#" }
	);
	m_zController->SetConverter<ScrfConverter>();
	m_zController->SetSaveCallback(std::bind(&PositionRotationSegment::OnCoordControllerSave, this, true));
	m_zController->SetValidateCallback(std::bind(&PositionRotationSegment::ValidateCoord, std::placeholders::_1, true));

	// Auto Z
	UiModule::Text* autoZText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_autoZController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		autoZText,
		std::vector<bool>{ false, true },
		selectedAutoZ,
		UiModule::SelectControllerOptions{ L"Auto Z: #", L"#" }
	);
	m_autoZController->SetConverter<EnabledDisabledConverter>();
	m_autoZController->SetSaveCallback(std::bind(&PositionRotationSegment::OnAutoZControllerSave, this, std::placeholders::_1));

	// Rotation
	UiModule::Text* rotationText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_rotationController = m_menuController->CreateLatestItemController<UiModule::EditableController<short>>(
		rotationText,
		selectedRotation,
		UiModule::EditableControllerOptions{ L"Rotation: #", L"#" }
	);
	m_rotationController->SetConverter<GtaAngleConverter<>>();
	m_rotationController->SetSaveCallback(std::bind(&PositionRotationSegment::OnCoordControllerSave, this, false));

	return true;
}

void ModMenuModule::PositionRotationSegment::Detach()
{
	if (m_persistencePrefix.size()) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

		persistence->Save(m_persistencePrefix + "_SelectedDoUpdatePosition", m_doUpdatePositionController->GetValue().value());

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
		if (m_rotationController && m_rotationController->GetValue().has_value()) {
			persistence->Save(m_persistencePrefix + "_SelectedRotation", m_rotationController->GetValue().value());
		}
	}

	DestroySegment();
}

void ModMenuModule::PositionRotationSegment::OnShow()
{
	SetEventListener<ModMenuModule::PlayerPosUpdateEvent>(&PositionRotationSegment::OnPlayerPosUpdate, m_doUpdatePosition);
	SetEventListener<ModMenuModule::PlayerRotUpdateEvent>(&PositionRotationSegment::OnPlayerRotUpdate, m_doUpdatePosition);
	if (m_doUpdatePosition) ForceUpdatePosition();
	else ApplyAutoZIfNeeded();
}

void ModMenuModule::PositionRotationSegment::OnHide()
{
	if (HasEventListener<ModMenuModule::PlayerPosUpdateEvent>()) {
		RemoveEventListener<ModMenuModule::PlayerPosUpdateEvent>();
	}
	if (HasEventListener<ModMenuModule::PlayerRotUpdateEvent>()) {
		RemoveEventListener<ModMenuModule::PlayerRotUpdateEvent>();
	}
}

bool ModMenuModule::PositionRotationSegment::ValidateCoord(Game::SCR_f value, bool isZCoord)
{
	if (value <= Game::Utils::FromFloat(0.0f)) return false;
	if (isZCoord) return value < Game::Utils::FromFloat(8.0f);
	else return value < Game::Utils::FromFloat(255.0f);
}

void ModMenuModule::PositionRotationSegment::SetCoordControllerValues(const Game::SCR_Vector3& position)
{
	if (!m_xController || !m_yController || !m_zController) return;

	m_xController->SetValue(position.x);
	m_yController->SetValue(position.y);
	m_zController->SetValue(position.z);
}

std::optional<Game::SCR_Vector3> ModMenuModule::PositionRotationSegment::GetCoordControllerValues() const
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

void ModMenuModule::PositionRotationSegment::OnPlayerPosUpdate(ModMenuModule::PlayerPosUpdateEvent& event)
{
	if (!m_doUpdatePosition) {
		return;
	}

	const auto& position = event.GetPosition();
	if (!position.has_value()) {
		return;
	}

	SetCoordControllerValues(position.value());
}

void ModMenuModule::PositionRotationSegment::OnPlayerRotUpdate(ModMenuModule::PlayerRotUpdateEvent& event)
{
	if (!m_doUpdatePosition) {
		return;
	}

	const auto& rotation = event.GetRotation();
	if (!rotation.has_value()) {
		return;
	}

	m_rotationController->SetValue(rotation.value());
}

void ModMenuModule::PositionRotationSegment::ApplyAutoZIfNeeded()
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

void ModMenuModule::PositionRotationSegment::ForceUpdatePosition()
{
	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();
	if (!playerPosCheat->IsEnabled()) return;

	const auto& position = playerPosCheat->GetLastPosition();
	if (position.has_value()) {
		SetCoordControllerValues(position.value());
	}

	const auto& rotation = playerPosCheat->GetLastRotation();
	if (rotation.has_value()) {
		m_rotationController->SetValue(rotation.value());
	}
}

void ModMenuModule::PositionRotationSegment::OnDoUpdatePositionControllerSave(bool newValue)
{
	if (m_doUpdatePosition == newValue) return;
	m_doUpdatePosition = newValue;

	if (!IsVisible()) return;

	if (newValue) {
		AddEventListener<ModMenuModule::PlayerPosUpdateEvent>(&PositionRotationSegment::OnPlayerPosUpdate);
		AddEventListener<ModMenuModule::PlayerRotUpdateEvent>(&PositionRotationSegment::OnPlayerRotUpdate);
		ForceUpdatePosition();
	}
	else {
		RemoveEventListener<ModMenuModule::PlayerPosUpdateEvent>();
		RemoveEventListener<ModMenuModule::PlayerRotUpdateEvent>();
	}
}

void ModMenuModule::PositionRotationSegment::OnCoordControllerSave(bool isZCoord)
{
	SetDoUpdatePosition(false);

	if (isZCoord) {
		m_autoZController->SetValue(false);
	}
	else {
		ApplyAutoZIfNeeded();
	}
}

void ModMenuModule::PositionRotationSegment::OnAutoZControllerSave(bool newValue)
{
	if (newValue) ApplyAutoZIfNeeded();
}

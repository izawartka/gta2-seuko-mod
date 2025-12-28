#include "camera-pos-cheat-coord-segment.h"
#include "../../../converters/camera-pos-cheat-mode.h"
#include "../../../converters/scrf.h"
#include "../root.h"

ModMenuModule::CameraPosCheatCoordSegment::CameraPosCheatCoordSegment(const std::wstring& coordLabel, size_t coordIndex)
	: m_coordLabel(coordLabel), m_coordIndex(coordIndex) 
{
	m_modeResolver = [this]() {
		CameraPosCheatCoordinate coordOptions = GetCurrentCoordinateOptions();
		return coordOptions.mode;
	};
}

ModMenuModule::CameraPosCheatCoordSegment::~CameraPosCheatCoordSegment()
{

}

bool ModMenuModule::CameraPosCheatCoordSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	CameraPosCheat* cameraPosCheat = CameraPosCheat::GetInstance();

	m_horCont = uiRoot->AddComponent<UiModule::HorCont>(m_vertCont);

	// mode
	UiModule::Text* modeText = m_menuController->CreateItem<UiModule::Text>(m_horCont, L"", options.textSize);
	auto modeController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<CameraPosCheatMode, CameraPosCheatMode>>(
		modeText,
		m_modeResolver,
		CameraPosCheat::GetAllCameraPosCheatModes(),
		UiModule::VarTextSelectControllerOptions{ m_coordLabel + L"#", L"#" }
	);
	modeController->SetConverter<CameraPosCheatModeConverter>();
	modeController->SetCustomSaveCallback([cameraPosCheat, this](CameraPosCheatMode newMode) {
		CameraPosCheatOptions options = cameraPosCheat->GetOptions();
		CameraPosCheatCoordinate* coordinate = const_cast<CameraPosCheatCoordinate*>(GetCoordinate(options, m_coordIndex));
		coordinate->mode = newMode;
		if (newMode == CameraPosCheatMode::LockTargetAt) {
			const auto& lastPos = cameraPosCheat->GetLastPosition();
			Game::SCR_f lastPosCoord = GetPosCoordinate(lastPos, m_coordIndex);
			coordinate->value = lastPosCoord;
		} else {
			coordinate->value = Game::Utils::FromFloat(0.0f);
		}
		cameraPosCheat->SetOptions(options);
	});
	m_modeMenuItemId = m_menuController->GetLatestMenuItemId();

	return true;
}

void ModMenuModule::CameraPosCheatCoordSegment::Detach()
{
	SetValueVisible(false);
	DestroySegment();
}

void ModMenuModule::CameraPosCheatCoordSegment::OnShow()
{
	m_watchedMode = Core::WatchManager::GetInstance()->Watch<UiModule::UpdateUIEvent>(
		m_modeResolver,
		this,
		&CameraPosCheatCoordSegment::OnModeChange
	);
}

void ModMenuModule::CameraPosCheatCoordSegment::OnHide()
{
	if (m_watchedMode) {
		Core::WatchManager::GetInstance()->Unwatch(m_watchedMode);
		m_watchedMode = nullptr;
	}
}

const ModMenuModule::CameraPosCheatCoordinate* ModMenuModule::CameraPosCheatCoordSegment::GetCoordinate(const CameraPosCheatOptions& cheatOptions, size_t coordIndex)
{
	switch (coordIndex) {
	case 0:
		return &cheatOptions.x;
	case 1:
		return &cheatOptions.y;
	case 2:
		return &cheatOptions.z;
	case 3:
		return &cheatOptions.zoom;
	default:
		return nullptr;
	}
}

Game::SCR_f ModMenuModule::CameraPosCheatCoordSegment::GetPosCoordinate(const std::optional<Game::CameraPos>& position, size_t coordIndex)
{
	switch (coordIndex) {
	case 0:
		return position.has_value() ? position->x : Game::Utils::FromFloat(0.0f);
	case 1:
		return position.has_value() ? position->y : Game::Utils::FromFloat(0.0f);
	case 2:
		return position.has_value() ? position->z : Game::Utils::FromFloat(0.0f);
	case 3:
		return position.has_value() ? position->zoom : Game::Utils::FromFloat(1.0f);
	default:
		return Game::Utils::FromFloat(1.0f);
	}
}

ModMenuModule::CameraPosCheatCoordinate ModMenuModule::CameraPosCheatCoordSegment::GetCurrentCoordinateOptions() const
{
	CameraPosCheat* cameraPosCheat = CameraPosCheat::GetInstance();
	const CameraPosCheatOptions& cheatOptions = cameraPosCheat->GetOptions();
	return *GetCoordinate(cheatOptions, m_coordIndex);
}

void ModMenuModule::CameraPosCheatCoordSegment::OnModeChange(
	const std::optional<ModMenuModule::CameraPosCheatMode>& prevMode, 
	const std::optional<ModMenuModule::CameraPosCheatMode>& newMode
) {
	SetValueVisible(newMode.has_value() && newMode.value() != CameraPosCheatMode::Unmodified);
}

void ModMenuModule::CameraPosCheatCoordSegment::SetValueVisible(bool visible)
{
	if (visible == (m_valueController != nullptr)) return;

	if (visible) {
		UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
		CameraPosCheat* cameraPosCheat = CameraPosCheat::GetInstance();
		const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

		// value
		size_t valueIndex = m_menuController->GetItemIndex(m_modeMenuItemId) + 1;
		m_menuController->SetNextAddedItemIndex(valueIndex);
		UiModule::Text* valueText = m_menuController->CreateItem<UiModule::Text>(m_horCont, L"", options.textSize);
		m_valueController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>>(
			valueText,
			[cameraPosCheat, this]() {
				const CameraPosCheatOptions& cheatOptions = cameraPosCheat->GetOptions();
				const CameraPosCheatCoordinate* coordinate = GetCoordinate(cheatOptions, m_coordIndex);
				return coordinate->value;
			},
			UiModule::VarTextEditableControllerOptions{ L"#", L"#" }
		);
		m_valueController->SetConverter<ScrfConverter>();
		m_valueController->SetCustomSaveCallback([cameraPosCheat, this](Game::SCR_f newValue) {
			CameraPosCheatOptions options = cameraPosCheat->GetOptions();
			CameraPosCheatCoordinate* coordinate = const_cast<CameraPosCheatCoordinate*>(GetCoordinate(options, m_coordIndex));
			coordinate->value = newValue;
			cameraPosCheat->SetOptions(options);
		});
		m_valueMenuItemId = m_menuController->GetLatestMenuItemId();
	}
	else {
		if (m_valueController) {
			m_menuController->DeleteItem(m_valueMenuItemId);
			m_valueController = nullptr;
			m_valueMenuItemId = -1;
		}
	}
}

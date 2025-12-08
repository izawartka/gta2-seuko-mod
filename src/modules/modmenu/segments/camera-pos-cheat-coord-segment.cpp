#include "camera-pos-cheat-coord-segment.h"
#include "../../../converters/camera-pos-cheat-mode.h"
#include "../../../converters/scrf.h"
#include "../root.h"

ModMenuModule::CameraPosCheatCoordSegment::CameraPosCheatCoordSegment(const std::wstring& coordLabel, size_t coordIndex)
	: m_coordLabel(coordLabel), m_coordIndex(coordIndex) { }

ModMenuModule::CameraPosCheatCoordSegment::~CameraPosCheatCoordSegment()
{
}

bool ModMenuModule::CameraPosCheatCoordSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	ModMenuModule::RootModule* root = ModMenuModule::RootModule::GetInstance();
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	CameraPosCheat* cameraPosCheat = root->GetCheat<CameraPosCheat>();

	m_horCont = uiRoot->AddComponent<UiModule::HorCont>(m_vertCont);

	// mode
	UiModule::Text* modeText = m_menuController->CreateItem<UiModule::Text>(m_horCont, L"", options.textSize);
	auto modeController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<CameraPosCheatMode, CameraPosCheatMode>>(
		modeText,
		[cameraPosCheat, this]() {
			const CameraPosCheatOptions& cheatOptions = cameraPosCheat->GetOptions();
			const CameraPosCheatCoordinate* coordinate = GetCoordinate(&cheatOptions, m_coordIndex);
			CameraPosCheatMode mode = coordinate->mode;
			SetValueVisible(mode != CameraPosCheatMode::Unmodified);
			return mode;
		},
		CameraPosCheat::GetAllCameraPosCheatModes(),
		UiModule::VarTextSelectControllerOptions{ m_coordLabel + L"#", L"#" }
	);
	modeController->SetConverter<CameraPosCheatModeConverter>();
	modeController->SetCustomSaveCallback([cameraPosCheat, this](CameraPosCheatMode newMode) {
		CameraPosCheatOptions options = cameraPosCheat->GetOptions();
		CameraPosCheatCoordinate* coordinate = const_cast<CameraPosCheatCoordinate*>(GetCoordinate(&options, m_coordIndex));
		coordinate->mode = newMode;
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
}

void ModMenuModule::CameraPosCheatCoordSegment::OnHide()
{
}

const ModMenuModule::CameraPosCheatCoordinate* ModMenuModule::CameraPosCheatCoordSegment::GetCoordinate(const CameraPosCheatOptions* cameraPosCheat, size_t coordIndex)
{
	switch (coordIndex) {
	case 0:
		return &cameraPosCheat->x;
	case 1:
		return &cameraPosCheat->y;
	case 2:
		return &cameraPosCheat->z;
	case 3:
		return &cameraPosCheat->zoom;
	default:
		return nullptr;
	}
}

void ModMenuModule::CameraPosCheatCoordSegment::SetValueVisible(bool visible)
{
	if (visible == (m_valueController != nullptr)) return;

	if (visible) {
		UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
		ModMenuModule::RootModule* root = ModMenuModule::RootModule::GetInstance();
		CameraPosCheat* cameraPosCheat = root->GetCheat<CameraPosCheat>();
		const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

		// value
		size_t valueIndex = m_menuController->GetItemIndex(m_modeMenuItemId) + 1;
		m_menuController->SetNextAddedItemIndex(valueIndex);
		UiModule::Text* valueText = m_menuController->CreateItem<UiModule::Text>(m_horCont, L"", options.textSize);
		m_valueController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::SCR_f, Game::SCR_f>>(
			valueText,
			[cameraPosCheat, this]() {
				const CameraPosCheatOptions& cheatOptions = cameraPosCheat->GetOptions();
				const CameraPosCheatCoordinate* coordinate = GetCoordinate(&cheatOptions, m_coordIndex);
				return coordinate->value;
			},
			UiModule::VarTextEditableControllerOptions{ m_coordLabel + L"#", L"#" }
		);
		m_valueController->SetConverter<ScrfConverter>();
		m_valueController->SetCustomSaveCallback([cameraPosCheat, this](Game::SCR_f newValue) {
			CameraPosCheatOptions options = cameraPosCheat->GetOptions();
			CameraPosCheatCoordinate* coordinate = const_cast<CameraPosCheatCoordinate*>(GetCoordinate(&options, m_coordIndex));
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

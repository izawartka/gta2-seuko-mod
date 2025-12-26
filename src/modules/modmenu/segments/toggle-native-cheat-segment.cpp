#include "toggle-native-cheat-segment.h"
#include "../../../converters/native-cheat-category.h"
#include "../../../converters/native-cheat.h"
#include "../../../converters/toggle-native-cheat-mode.h"
#include "../root.h"

std::optional<ModMenuModule::ToggleNativeCheatSegmentData> ModMenuModule::ToggleNativeCheatSegment::GetSegmentData() const
{
	if (!m_cheatController || !m_modeController) {
		spdlog::error("Cannot get segment data: controllers are not initialized");
		return std::nullopt;
	}

	const auto& cheatAddressOpt = m_cheatController->GetValue();
	if (!cheatAddressOpt.has_value()) {
		spdlog::error("Cannot get segment data: cheatAddress has no value");
		return std::nullopt;
	}

	const auto& modeOpt = m_modeController->GetValue();
	if (!modeOpt.has_value()) {
		spdlog::error("Cannot get segment data: mode has no value");
		return std::nullopt;
	}

	return ToggleNativeCheatSegmentData{
		cheatAddressOpt.value(),
		modeOpt.value()
	};
}

bool ModMenuModule::ToggleNativeCheatSegment::SetSegmentData(const ToggleNativeCheatSegmentData& data)
{
	if (!m_cheatController || !m_modeController) {
		spdlog::error("Cannot set segment data: controllers are not initialized");
		return false;
	}

	NativeCheatsKeeperCheat* nativeCheatsKeeper = NativeCheatsKeeperCheat::GetInstance();
	const auto* cheatDef = NativeCheatsKeeperCheat::GetNativeCheatDef(data.cheatAddress);
	if (!cheatDef) {
		spdlog::error("Cannot set segment data: no native cheat found for address {}", data.cheatAddress);
		return false;
	}

	m_cheatCategoryController->SetValue(cheatDef->category);
	DestroyCheatController();
	CreateCheatController(cheatDef->category);
	m_cheatController->SetValue(data.cheatAddress);
	m_modeController->SetValue(data.mode);

	return true;
}

bool ModMenuModule::ToggleNativeCheatSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	NativeCheatsKeeperCheat* nativeCheatsKeeper = NativeCheatsKeeperCheat::GetInstance();
	const auto& cheatCategoriesList = NativeCheatsKeeperCheat::GetAllNativeCheatCategories();
	auto selectedCheatCategory = cheatCategoriesList[0];

	UiModule::Text* cheatCategoryText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_cheatCategoryController = m_menuController->CreateLatestItemController<UiModule::SelectController<NativeCheatCategory>>(
		cheatCategoryText,
		cheatCategoriesList,
		selectedCheatCategory,
		UiModule::SelectControllerOptions{ L"Cheat category: #", L"#" }
	);
	m_cheatCategoryController->SetConverter<NativeCheatCategoryConverter>();
	m_cheatCategoryController->SetSaveCallback(std::bind(&ToggleNativeCheatSegment::OnCheatCategoryControllerSave, this, std::placeholders::_1));

	m_cheatText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_cheatMenuItemId = m_menuController->GetLatestMenuItemId();
	CreateCheatController(selectedCheatCategory);

	const auto& toggleModes = Utils::ToggleNativeCheat::GetAllToggleNativeCheatModes();
	auto selectedToggleMode = toggleModes[0];

	UiModule::Text* modeText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_modeController = m_menuController->CreateLatestItemController<UiModule::SelectController<Utils::ToggleNativeCheat::ToggleNativeCheatMode>>(
		modeText,
		toggleModes,
		selectedToggleMode,
		UiModule::SelectControllerOptions{ L"Mode: #", L"#" }
	);
	m_modeController->SetConverter<ToggleNativeCheatModeConverter>();

	return true;
}

void ModMenuModule::ToggleNativeCheatSegment::Detach()
{
	m_cheatMenuItemId = -1;
	DestroySegment();
}

void ModMenuModule::ToggleNativeCheatSegment::CreateCheatController(NativeCheatCategory category)
{
	if (m_cheatMenuItemId == -1 || !m_cheatText) return;
	if (m_cheatController) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	NativeCheatsKeeperCheat* nativeCheatsKeeper = NativeCheatsKeeperCheat::GetInstance();
	const auto& cheatsInCategory = NativeCheatsKeeperCheat::GetAllNativeCheatsByCategory(category);
	if (cheatsInCategory.size() == 0) {
		spdlog::error("Cannot create cheat controller: no cheats found in category {}", static_cast<int>(category));
		return;
	}
	auto selectedCheat = cheatsInCategory[0];

	m_cheatController = m_menuController->CreateItemController<UiModule::SelectController<size_t>>(
		m_cheatMenuItemId,
		m_cheatText,
		cheatsInCategory,
		selectedCheat,
		UiModule::SelectControllerOptions{ L"Cheat: #", L"#" }
	);
	m_cheatController->SetConverter<NativeCheatConverter>();
}

void ModMenuModule::ToggleNativeCheatSegment::DestroyCheatController()
{
	if (!m_cheatController) return;
	m_menuController->DeleteItemController(m_cheatMenuItemId);
	m_cheatController = nullptr;
}

void ModMenuModule::ToggleNativeCheatSegment::OnCheatCategoryControllerSave(NativeCheatCategory newCategory)
{
	DestroyCheatController();
	CreateCheatController(newCategory);
}

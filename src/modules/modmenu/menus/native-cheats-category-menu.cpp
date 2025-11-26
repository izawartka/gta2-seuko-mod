#include "native-cheats-category-menu.h"
#include "../cheats/native-cheats-keeper.h"
#include "../../../converters/enabled-disabled.h"
#include "../../../converters/in-brackets.h"
#include "../../../converters/native-cheat-state.h"
#include "../root.h"

ModMenuModule::NativeCheatsCategoryMenu::NativeCheatsCategoryMenu(const NativeCheatCategoryDef& categoryDef, size_t page) :
	m_categoryDef(categoryDef),
	m_cheats(NativeCheatsKeeperCheat::GetAllNativeCheatsByCategory(categoryDef.category))
{
	m_page = page;
	m_pageCount = (m_cheats.size() + CHEATS_PER_PAGE - 1) / CHEATS_PER_PAGE;
}

ModMenuModule::NativeCheatsCategoryMenu::~NativeCheatsCategoryMenu()
{
}

bool ModMenuModule::NativeCheatsCategoryMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(GetPageTitle(), vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	if (m_page > 0) {
		m_menuController->CreateItem<UiModule::Text>(vertCont, L"Previous page", options.textSize);
		m_prevPageItemId = m_menuController->GetLatestMenuItemId();
	}
	if (m_page < m_pageCount - 1) {
		m_menuController->CreateItem<UiModule::Text>(vertCont, L"Next page", options.textSize);
		m_nextPageItemId = m_menuController->GetLatestMenuItemId();
	}

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	std::vector<NativeCheatState> cheatStates = {
		NativeCheatState::ForceDisabled,
		NativeCheatState::Unchanged,
		NativeCheatState::ForceEnabled
	};

	auto* nativeCheatsKeeper = this->GetCheat<NativeCheatsKeeperCheat>();

	for (size_t i = m_page * CHEATS_PER_PAGE; i < std::min(m_cheats.size(), (m_page + 1) * CHEATS_PER_PAGE); i++) {
		const NativeCheatDef& cheat = m_cheats[i];

		auto cheatCont = m_menuController->CreateItem<UiModule::HorCont>(vertCont);
		auto cheatValueText = uiRoot->AddComponent<UiModule::Text>(cheatCont, L"", options.textSize);
		auto cheatValueResolver = [nativeCheatsKeeper, cheat]() {
			return nativeCheatsKeeper->IsCheatEnabled(cheat);
		};
		auto cheatValueController = uiRoot->AddController<UiModule::VarTextController<bool, bool>>(
			cheatValueText, 
			cheatValueResolver,
			UiModule::VarTextControllerOptions{ cheat.name + L": #", L"#" }
		);
		cheatValueController->SetConverter<EnabledDisabledConverter>();
		m_cheatValueControllers.push_back(cheatValueController);

		auto cheatStateText = uiRoot->AddComponent<UiModule::Text>(cheatCont, L"", options.textSize);
		auto cheatStateResolver = [nativeCheatsKeeper, cheat]() {
			return nativeCheatsKeeper->GetCheatState(cheat);
		};
		auto cheatStateController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<NativeCheatState, NativeCheatState>>(
			cheatStateText,
			cheatStateResolver,
			cheatStates,
			UiModule::VarTextSelectControllerOptions{ L"#", L"#" }
		);
		cheatStateController->SetConverter<InBracketsConverter<NativeCheatState, NativeCheatStateConverter<false>>>();
		cheatStateController->SetCustomSaveCallback([nativeCheatsKeeper, cheat](NativeCheatState newValue) {
			nativeCheatsKeeper->SetEnabled(true);
			nativeCheatsKeeper->SetCheat(cheat, newValue);
		});
	}

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::NativeCheatsCategoryMenu::Detach()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	for (auto& controller : m_cheatValueControllers) {
		uiRoot->RemoveController(controller);
	}
	m_cheatValueControllers.clear();

	DestroyMenu();
}

void ModMenuModule::NativeCheatsCategoryMenu::OnShow()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	for (auto& controller : m_cheatValueControllers) {
		controller->SetWatching(true);
	}
}

void ModMenuModule::NativeCheatsCategoryMenu::OnHide()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	for (auto& controller : m_cheatValueControllers) {
		controller->SetWatching(false);
	}
}

void ModMenuModule::NativeCheatsCategoryMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	ModMenuModule::MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();

	if (id == 0) {
		// Go back
		menuManager->RemoveLastMenu();
		return;
	} else if (id == m_prevPageItemId) {
		// Previous page
		MenuId thisMenuId = menuManager->GetTopMenuId();
		menuManager->AddMenu<NativeCheatsCategoryMenu>(m_categoryDef, m_page - 1);
		menuManager->RemoveMenu(thisMenuId);
		return;
	} else if (id == m_nextPageItemId) {
		// Next page
		MenuId thisMenuId = menuManager->GetTopMenuId();
		menuManager->AddMenu<NativeCheatsCategoryMenu>(m_categoryDef, m_page + 1);
		menuManager->RemoveMenu(thisMenuId);
		return;
	}
}

std::wstring ModMenuModule::NativeCheatsCategoryMenu::GetPageTitle() const
{
	if (m_pageCount > 1) {
		return L"#Native cheats - " + m_categoryDef.name + L" (" + std::to_wstring(m_page + 1) + L"/" + std::to_wstring(m_pageCount) + L")#";
	}
	else {
		return L"#Native cheats - " + m_categoryDef.name + L"#";
	}
}

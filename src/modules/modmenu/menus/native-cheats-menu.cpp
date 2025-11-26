#include "native-cheats-menu.h"
#include "../cheats/native-cheats-keeper.h"
#include "native-cheats-category-menu.h"
#include "../root.h"

ModMenuModule::NativeCheatsMenu::NativeCheatsMenu()
{

}

ModMenuModule::NativeCheatsMenu::~NativeCheatsMenu()
{

}

bool ModMenuModule::NativeCheatsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Native cheats#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// reset all cheats
	auto resetAllText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Reset all cheats", options.textSize);
	auto resetAllBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(resetAllText);
	resetAllBtn->SetCallback([this]() {
		auto nativeCheatsKeeper = this->GetCheat<NativeCheatsKeeperCheat>();
		nativeCheatsKeeper->ResetAll();
		nativeCheatsKeeper->SetEnabled(false);
	});

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	// categories
	for (const auto& category : m_categories) {
		auto categoryText = m_menuController->CreateItem<UiModule::Text>(vertCont, category.name.c_str(), options.textSize);
		if (m_firstCategoryItemId == -1) {
			m_firstCategoryItemId = m_menuController->GetLatestMenuItemId();
		}
	}

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::NativeCheatsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		return;
	default:
		break;
	}

	if (id >= m_firstCategoryItemId && id < static_cast<UiModule::MenuItemId>(m_categories.size() + m_firstCategoryItemId)) {
		const NativeCheatCategoryDef& categoryDef = m_categories.at(id - m_firstCategoryItemId);
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::NativeCheatsCategoryMenu>(categoryDef);
		return;
	}
}

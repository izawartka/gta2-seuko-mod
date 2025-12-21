#include "version-menu.h"
#include "../root.h"
#include "../../../version.h"

ModMenuModule::VersionMenu::VersionMenu()
{

}

ModMenuModule::VersionMenu::~VersionMenu()
{

}

bool ModMenuModule::VersionMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Version details#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);
	std::wstring versionStr = L"Version: #" SEUKOMOD_VERSION_WSTR L"#";
	m_menuController->CreateItem<UiModule::Text>(vertCont, versionStr, options.textSize);
	std::wstring gitHashStr = L"Git commit: #" SEUKOMOD_GIT_WSTR L"#";
	m_menuController->CreateItem<UiModule::Text>(vertCont, gitHashStr, options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::VersionMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

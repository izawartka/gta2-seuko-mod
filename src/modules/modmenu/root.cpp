#include "root.h"
#include "cheat-registry.h"

ModMenuModule::RootModule* ModMenuModule::RootModule::m_instance = nullptr;

ModMenuModule::RootModule::RootModule(ModMenuModule::ModMenuOptions options)
	: m_options(options)
{
	assert(UiModule::RootModule::GetInstance() != nullptr, "ModMenu::RootModule requires Ui::RootModule to be initialized first!");
	assert(m_instance == nullptr, "ModMenu::RootModule instance already exists!");
	m_instance = this;
	spdlog::info("ModMenu::RootModule module initialized.");
}

ModMenuModule::RootModule::~RootModule()
{
	m_instance = nullptr;
}

ModMenuModule::RootModule* ModMenuModule::RootModule::GetInstance()
{
	return m_instance;
}

bool ModMenuModule::RootModule::Attach()
{
	AddEventListener<KeyDownEvent>(&RootModule::OnKeyDown);
	spdlog::info("ModMenu::RootModule module attached.");

	InstantiateCheats();
	return true;
}

void ModMenuModule::RootModule::Detach()
{
	RemoveAllEventListeners();
	ClearMenus();
	spdlog::info("ModMenu::RootModule module detached.");
}

void ModMenuModule::RootModule::RemoveLastMenu()
{
	if (m_menus.empty()) {
		spdlog::warn("Attempted to remove menu from empty stack");
		return;
	}
	MenuBase* backMenu = m_menus.back().get();
	backMenu->SetVisible(false);
	backMenu->Detach();
	m_menus.pop_back();

	if (m_menus.empty()) {
		return;
	}

	ModMenuModule::MenuBase* menu = m_menus.back().get();
	menu->Attach();
	menu->SetVisible(m_visible);
}

void ModMenuModule::RootModule::ClearMenus()
{
	while (!m_menus.empty()) {
		MenuBase* backMenu = m_menus.back().get();
		backMenu->SetVisible(false);
		backMenu->Detach();
		m_menus.pop_back();
	}
}

void ModMenuModule::RootModule::SetVisible(bool visible)
{
	if (visible == m_visible) {
		return;
	}
	m_visible = visible;

	if (m_menus.empty()) {
		return;
	}

	m_menus.back()->SetVisible(visible);
}

void ModMenuModule::RootModule::InstantiateCheats()
{
	auto& factories = ModMenuModule::CheatRegistry::Factories();
	spdlog::info("Instantiating {} cheats", factories.size());
	for (auto& factory : factories) {
		ModMenuModule::CheatBase* cheat = factory();
		if (!cheat) {
			spdlog::error("Cheat factory returned null pointer");
			continue;
		}

		if (!cheat->Attach()) {
			spdlog::error("Failed to attach cheat {}", typeid(*cheat).name());
			delete cheat;
			continue;
		}

		std::type_index typeIdx(typeid(*cheat));
		if (m_cheats.find(typeIdx) != m_cheats.end()) {
			spdlog::error("Cheat {} is already registered", typeid(*cheat).name());
			cheat->Detach();
			delete cheat;
			continue;
		}

		m_cheats[typeIdx] = std::unique_ptr<ModMenuModule::CheatBase>(cheat);
	}
}

void ModMenuModule::RootModule::OnKeyDown(KeyDownEvent& event)
{
	if (event.GetKeyCode() == m_options.toggleKey) {
		SetVisible(!m_visible);
	}
}
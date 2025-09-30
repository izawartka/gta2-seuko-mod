#include "root.h"

UiModule::RootModule* UiModule::RootModule::m_instance = nullptr;

UiModule::RootModule::RootModule()
{
	assert(m_instance == nullptr, "UiModule::RootModule instance already exists!");
	m_instance = this;
	spdlog::info("UiModule::RootModule instance created");
}

UiModule::RootModule::~RootModule()
{
	ClearControllers();
	ClearComponents();
	m_instance = nullptr;
}

UiModule::RootModule* UiModule::RootModule::GetInstance()
{
	return m_instance;
}

bool UiModule::RootModule::Attach()
{
	AddEventListener<DrawUIEvent>(&UiModule::RootModule::OnDraw);
	spdlog::info("UiModule::RootModule attached");
	return true;
}

void UiModule::RootModule::Detach()
{
	RemoveAllEventListeners();
	spdlog::info("UiModule::RootModule detached");
}

void UiModule::RootModule::OnDraw(const DrawUIEvent& event)
{
	for (const auto& component : m_components) {
		component->DrawIfVisible();
	}
}

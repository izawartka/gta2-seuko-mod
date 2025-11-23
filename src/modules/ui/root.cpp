#include "root.h"

UiModule::RootModule* UiModule::RootModule::m_instance = nullptr;

UiModule::RootModule::RootModule()
{
	assert(KeyBindingModule::RootModule::GetInstance() != nullptr && "ModMenuModule::RootModule requires KeyBindingModule::RootModule to be initialized first!");
	assert(m_instance == nullptr && "UiModule::RootModule instance already exists!");
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

void UiModule::RootModule::RemoveController(Controller* controller)
{
	auto it = std::find_if(m_controllers.begin(), m_controllers.end(), [controller](const std::unique_ptr<Controller>& ptr) {
		return ptr.get() == controller;
	});
	if (it != m_controllers.end()) {
		std::unique_ptr<Controller> controllerPtr = std::move(*it);
		m_controllers.erase(it);
	}
	else {
		spdlog::error("Attempted to remove a controller that is not managed by RootModule");
	}
}

void UiModule::RootModule::ClearControllers()
{
	while (!m_controllers.empty()) {
		m_controllers.erase(m_controllers.begin());
	}
}

void UiModule::RootModule::OnDraw(DrawUIEvent& event)
{
	for (const auto& component : m_components) {
		component->DrawIfVisible();
	}
}

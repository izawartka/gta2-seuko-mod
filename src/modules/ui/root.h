#pragma once
#include "common.h"
#include "../../events/draw.h"
#include "component.h"
#include "controller.h"

namespace UiModule {
	class RootModule : public Core::ModuleBase {
	public:
		RootModule();
		virtual ~RootModule() override;
		static RootModule* GetInstance();
		bool Attach();
		void Detach();

		template<typename ComponentT, typename... Args>
		ComponentT* AddComponent(Args&&... args) {
			static_assert(std::is_base_of_v<Component, ComponentT>, "ComponentT must be derived from Component");
			ComponentT* component = new ComponentT(std::forward<Args>(args)...);
			m_components.emplace_back(std::unique_ptr<Component>(component));
			return component;
		}

		template<typename ComponentT>
		void RemoveComponent(ComponentT* component, bool removeChildren = false) {
			static_assert(std::is_base_of_v<Component, ComponentT>, "ComponentT must be derived from Component");
			auto it = std::find_if(m_components.begin(), m_components.end(), [component](const std::unique_ptr<Component>& ptr) {
				return ptr.get() == component;
			});
			if (it == m_components.end()) {
				spdlog::error("Attempted to remove a component that is not managed by RootModule");
				return;
			}

			if (removeChildren) {
				std::vector<Component*> children = component->GetChildren();
				for (Component* child : children) {
					RemoveComponent(child, true);
				}
			}

			if (it->get() != component) {
				it = std::find_if(m_components.begin(), m_components.end(), [component](const std::unique_ptr<Component>& ptr) {
					return ptr.get() == component;
				});
			}

			if (it != m_components.end()) {
				m_components.erase(it);
			}
			else {
				spdlog::error("Attempted to remove a component that, after removing children, is not managed by RootModule anymore");
			}
		}

		void ClearComponents() {
			while (!m_components.empty()) {
				m_components.pop_back();
			}
		}

		template<typename ControllerT, typename... Args>
		ControllerT* AddController(Args&&... args) {
			static_assert(std::is_base_of_v<Controller, ControllerT>, "ControllerT must be derived from Controller");
			ControllerT* controller = new ControllerT(std::forward<Args>(args)...);
			m_controllers.emplace_back(std::unique_ptr<Controller>(controller));
			return controller;
		}

		template<typename ControllerT>
		void RemoveController(ControllerT* controller) {
			static_assert(std::is_base_of_v<Controller, ControllerT>, "ControllerT must be derived from Controller");
			auto it = std::find_if(m_controllers.begin(), m_controllers.end(), [controller](const std::unique_ptr<Controller>& ptr) {
				return ptr.get() == controller;
			});
			if (it != m_controllers.end()) {
				m_controllers.erase(it);
			}
			else {
				spdlog::error("Attempted to remove a controller that is not managed by RootModule");
			}
		}

		void ClearControllers() {
			while (!m_controllers.empty()) {
				m_controllers.erase(m_controllers.begin());
			}
		}

	private:
		void OnDraw(DrawUIEvent& event);

		std::vector<std::unique_ptr<Component>> m_components = {};
		std::vector<std::unique_ptr<Controller>> m_controllers = {};
		static RootModule* m_instance;
	};
}
#pragma once
#include "../common.h"
#include "../controller.h"
#include "../components/selectable.h"
#include "../root.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	using MenuItemId = size_t;
	using MenuActionCallback = std::function<void(Selectable*, MenuItemId)>;

	struct MenuControllerOptions {
		bool loop = true;
		bool hideMarkerNonActive = false;
		Game::KeyCode keyPrev = Game::KeyCode::DIK_LBRACKET;
		Game::KeyCode keyNext = Game::KeyCode::DIK_RBRACKET;
		Game::KeyCode keyAction = Game::KeyCode::DIK_BACKSLASH;
		SelectableOptions createdSelectableOptions = {};
	};

	class MenuController : public Controller, public Core::EventListenerSupport {
	public:
		MenuController(MenuControllerOptions options = {});
		virtual ~MenuController();

		template<typename ComponentT, typename... Args>
		ComponentT* CreateItem(Component* parent, Args&&... otherArgs) {
			static_assert(std::is_base_of_v<Component, ComponentT>, "ComponentT must be derived from Component");
			Selectable* selectable = RootModule::GetInstance()->AddComponent<Selectable>(parent, m_options.createdSelectableOptions);
			ComponentT* item = RootModule::GetInstance()->AddComponent<ComponentT>(selectable, std::forward<Args>(otherArgs)...);
			AddItem(selectable);
			return item;
		}

		MenuItemId AddItem(Selectable* item);
		std::vector<MenuItemId> AddItems(const std::vector<Selectable*>& items);
		void RemoveItem(MenuItemId id);

		void SetActive(bool active);
		bool IsActive() const { return m_active; }

		void SetIndex(MenuItemId index);
		MenuItemId GetIndex() const { return m_currentIndex; }

		void Next();
		void Previous();
		void Action();

		void SetOnActionCallback(MenuActionCallback callback) { m_onAction = callback; }

	protected:
		void OnKeyDown(KeyDownEvent& event);

		MenuControllerOptions m_options;
		std::vector<Selectable*> m_items = {};
		MenuItemId m_currentIndex = 0;
		bool m_active = false;
		MenuActionCallback m_onAction = nullptr;
	};
}
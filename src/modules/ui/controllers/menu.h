#pragma once
#include "../common.h"
#include "../standard-binds-support.h"
#include "../controller.h"
#include "../components/selectable.h"
#include "../root.h"
#include "../../../events/keyboard.h"
#include "menu-item.h"

namespace UiModule {
	using MenuItemId = size_t;
	using MenuItemGroupId = size_t;
	using MenuActionCallback = std::function<void(Selectable*, MenuItemId)>;

	struct MenuControllerOptions {
		bool loop = true;
		bool hideMarkerNonActive = false;
		StandardBindsSupportOptions keyBindOptions = {};
		SelectableOptions createdSelectableOptions = {};
	};

	class MenuController : public Controller, public Core::EventListenerSupport, public StandardBindsSupport {
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

		template<typename MenuItemControllerT, typename... Args>
		MenuItemControllerT* CreateItemController(MenuItemId id, Args&&... args) {
			static_assert(std::is_base_of_v<MenuItemController, MenuItemControllerT>, "MenuItemControllerT must be derived from MenuItemController");

			MenuItem* menuItem = GetItemById(id);
			if (!menuItem) {
				return nullptr;
			}

			if (menuItem->controller != nullptr) {
				spdlog::error("MenuController::CreateItemController: Item with id {} already has a controller", id);
				return nullptr;
			}

			UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
			MenuItemControllerT* controller = uiRoot->AddController<MenuItemControllerT>(std::forward<Args>(args)...);
			controller->SetEditStopCallback(std::bind(&MenuController::OnItemEditStop, this));
			menuItem->controller = controller;
			return controller;
		}

		template<typename MenuItemControllerT, typename... Args>
		MenuItemControllerT* CreateLatestItemController(Args&&... args) {
			static_assert(std::is_base_of_v<MenuItemController, MenuItemControllerT>, "MenuItemControllerT must be derived from MenuItemController");

			if (m_nextItemId == 0) {
				spdlog::error("MenuController::CreateLastItemController: No items have been added yet");
				return nullptr;
			}

			return CreateItemController<MenuItemControllerT>(m_nextItemId - 1, std::forward<Args>(args)...);
		}

		template<typename MenuItemControllerT>
		MenuItemControllerT* RemoveItemController(MenuItemId id) {
			static_assert(std::is_base_of_v<MenuItemController, MenuItemControllerT>, "MenuItemControllerT must be derived from MenuItemController");

			return static_cast<MenuItemControllerT*>(RemoveItemController(id));
		}

		void AddItemController(MenuItemId id, MenuItemController* controller);
		void AddLatestItemController(MenuItemController* controller);
		MenuItemController* RemoveItemController(MenuItemId id);
		void DeleteItemController(MenuItemId id);
		MenuItemId GetLatestMenuItemId() const { return m_nextItemId - 1; }

		void SetNextAddedItemIndex(size_t index) { m_nextAddedItemIndex = index; }
		size_t GetNextAddedItemIndex() const { return m_nextAddedItemIndex; }

		MenuItemGroupId GetCurrentGroupId() const { return m_currentGroupId; }
		void SetCurrentGroupId(MenuItemGroupId groupId) { m_currentGroupId = groupId; }
		void DeleteGroupItems(MenuItemGroupId groupId);
		void DeleteCurrentGroupItems();

		void SetItemsWatching(bool watching);
		bool IsItemsWatching() const { return m_itemsWatching; }

		void SetActive(bool active);
		bool IsActive() const { return m_active; }

		void SetIndex(size_t index);
		size_t GetIndex() const { return m_currentIndex; }

		void Next();
		void Previous();
		void Action();

		void SetOnActionCallback(MenuActionCallback callback) { m_onAction = callback; }
		void OnItemEditStop();

	protected:
		void OnKeyDown(KeyDownEvent& event);

		struct MenuItem {
			MenuItemId id;
			Selectable* item;
			MenuItemController* controller;
			MenuItemGroupId groupId;
		};

		MenuItem* GetItemById(MenuItemId id);
		void SetActiveMenuControl(bool active);
		void SetActiveController(MenuItemController* controller);
		void UpdateIndex();

		MenuControllerOptions m_options;
		std::vector<MenuItem> m_items;
		MenuItemId m_nextItemId = 0;
		MenuItemGroupId m_currentGroupId = 0;
		size_t m_currentIndex = -1;
		size_t m_nextAddedItemIndex = 0;
		bool m_itemsWatching = false;
		bool m_active = false;
		bool m_activeMenuControl = false;
		MenuItemController* m_activeController = nullptr;
		MenuActionCallback m_onAction = nullptr;
	};
}
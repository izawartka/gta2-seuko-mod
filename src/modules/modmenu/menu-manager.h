#pragma once
#include "common.h"
#include "menu-base.h"
#include "../../events/keyboard.h"
#include "../../events/game-start.h"
#include "../../events/game-end.h"

namespace ModMenuModule {
	using MenuId = size_t;

	class MenuManager : public Core::EventListenerSupport {
	public:
		static MenuManager* GetInstance();

		template<typename MenuT, typename... Args>
		MenuId AddMenu(Args&&... args) {
			static_assert(std::is_base_of_v<MenuBase, MenuT>, "MenuT must be derived from MenuBase");
			MenuT* newMenu = new MenuT(std::forward<Args>(args)...);
			MenuId id = m_nextMenuId++;
			m_menuIds.push_back(id);
			PendingChange change = { ChangeType::Add, id, newMenu };
			AddPendingChange(change);
			return id;
		}

		void RemoveMenu(MenuId id);
		void RemoveLastMenu();
		void ClearMenus();
		void SetVisible(bool visible);

	private:
		friend class RootModule;
		MenuManager();
		virtual ~MenuManager();
		MenuManager(const MenuManager&) = delete;
		MenuManager& operator=(const MenuManager&) = delete;

		static MenuManager* m_instance;

		void OnKeyDown(KeyDownEvent& event);
		void OnGameStart(GameStartEvent& event);
		void OnGameEnd(PreGameEndEvent& event);
		void Attach();
		void Detach();

		enum class ChangeType { Add, Remove, UpdateVisible, DetachTop };
		struct PendingChange {
			ChangeType type;
			MenuId id = 0;
			MenuBase* menu = nullptr; // Only used for Add
		};

		std::queue<PendingChange> m_pendingChanges;

		void ApplyMenuAdd(PendingChange& change);
		void ApplyMenuRemove(PendingChange& change);
		void DetachTopMenu();
		void ProcessPendingChanges();
		void AddPendingChange(const PendingChange& change);

		struct MenuStackEntry {
			MenuId id;
			std::unique_ptr<MenuBase> menu;
		};

		std::vector<MenuId> m_menuIds;
		std::vector<MenuStackEntry> m_menus;
		bool m_visible = false;
		bool m_processingChanges = false;
		bool m_topMenuAttached = false;
		bool m_topMenuVisible = false;
		bool m_aboutToEndGame = false;
		bool m_inGame = false;
		MenuId m_nextMenuId = 1;
	};
}
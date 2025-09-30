#pragma once
#include "common.h"
#include "rect.h"

namespace UiModule {
	class Component {
	public:
		virtual ~Component();
		virtual void DrawIfVisible();
		virtual void Draw() {}

		virtual void AddChild(Component* child);
		virtual void RemoveChild(Component* child);
		virtual void UpdateChildrenPos();
		void SetPosition(Game::SCR_f x, Game::SCR_f y);

		Rect GetRect() const { return m_rect; }
		Component* GetParent() const { return m_parent; }
		std::vector<Component*> GetChildren() const { return m_children; }
		void OnChildSizeUpdate(Component* child);
		void SetVisible(bool visible);
		bool IsVisible() const { return m_visibleInherited; }
	protected:
		void UpdateSize(Game::SCR_f* width, Game::SCR_f* height);
		void UpdateChildrenVisibility();
		Component* m_parent = nullptr;
		std::vector<Component*> m_children = {};
		Rect m_rect = { 0, 0, 0, 0 };
		bool m_visible = true;
		bool m_visibleInherited = true;
	};
}
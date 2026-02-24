#pragma once
#include "../common.h"
#include "../mouse-button.h"
#include "../mouse-position.h"

namespace MouseModule {
	/*
	Base class for all mouse events. Not to be used directly.
	*/
	class MouseEvent : public Core::EventBase {
	public:
		static bool Init() { return false; }

	protected:
		virtual ~MouseEvent() override {};
	};

	/*
	Base class for all mouse button events. Not to be used directly.
	*/
	class MouseButtonEvent : public MouseEvent {
	public:
		static bool Init() { return false; }
		MouseButton GetButton() const { return m_button; }

	protected:
		MouseButtonEvent(MouseButton button) : m_button(button) {};
		virtual ~MouseButtonEvent() override {};

		MouseButton m_button;
	};

	/*
	Dispatched when a mouse button is pressed down.
	*/
	class MouseButtonDownEvent : public MouseButtonEvent {
	public:
		static bool Init();
		static void Deinit();
		MouseButtonDownEvent(MouseButton button) : MouseButtonEvent(button) {}
		virtual ~MouseButtonDownEvent() override {};
	};

	/*
	Dispatched when a mouse button is released.
	*/
	class MouseButtonUpEvent : public MouseButtonEvent {
	public:
		static bool Init();
		static void Deinit();
		MouseButtonUpEvent(MouseButton button) : MouseButtonEvent(button) {}
		virtual ~MouseButtonUpEvent() override {};
	};

	/*
	Dispatched when the mouse is moved.
	*/
	class MouseMoveEvent : public MouseEvent {
	public:
		static bool Init();
		static void Deinit();
		MouseMoveEvent(MousePosition position) : m_position(position) {}
		virtual ~MouseMoveEvent() override {};

		MousePosition GetPosition() const { return m_position; }

	private:
		MousePosition m_position;
	};

	/*
	Dispatched when the mouse is moved while the cursor is locked. 
	Contains the relative movement of the move.
	*/
	class MouseLockedMoveEvent : public MouseEvent {
	public:
		static bool Init();
		static void Deinit();
		MouseLockedMoveEvent(MousePosition delta) : m_delta(delta) {}
		virtual ~MouseLockedMoveEvent() override {};

		MousePosition GetDelta() const { return m_delta; }

	private:
		MousePosition m_delta;
	};

	/*
	Dispatched when the mouse leaves the game window.
	*/
	class MouseLeaveEvent : public MouseEvent {
		public:
		static bool Init();
		static void Deinit();
		MouseLeaveEvent() {}
		virtual ~MouseLeaveEvent() override {};
	};

	/*
	Dispatched when the mouse enters the game window.
	*/
	class MouseEnterEvent : public MouseEvent {
		public:
		static bool Init();
		static void Deinit();
		MouseEnterEvent() {}
		virtual ~MouseEnterEvent() override {};
	};
}

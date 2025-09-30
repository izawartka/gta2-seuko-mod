#pragma once
#include "common.h"
#include "types.h"
#include "hook-base.h"
#include "hook-manager.h"
#include "event-base.h"
#include "event-manager.h"
#include "event-listener-support.h"
#include "make-resolver.h"
#include "watched-base.h"
#include "watch-manager.h"
#include "module-base.h"
#include "module-manager.h"

namespace Core {
	class Core
	{
	public:
		Core();
		~Core();
		Core(const Core&) = delete;
		Core& operator=(const Core&) = delete;

	private:
		static Core* m_instance;
		EventManager m_eventManager;
		HookManager m_hookManager;
		WatchManager m_watchManager;
		ModuleManager m_moduleManager;
	};
}

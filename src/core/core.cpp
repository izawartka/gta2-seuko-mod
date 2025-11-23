#include "core.h"

Core::Core* Core::Core::m_instance = nullptr;

Core::Core::Core()
{
	assert(m_instance == nullptr && "Core instance already exists!");
	m_instance = this;
	spdlog::info("Done initializing Core.");
}

Core::Core::~Core()
{
	m_instance = nullptr;
}

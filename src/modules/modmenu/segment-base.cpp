#include "segment-base.h"
#include "root.h"

bool ModMenuModule::SegmentBase::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);
	return false;
}

void ModMenuModule::SegmentBase::Detach()
{
	DestroySegment();
}

void ModMenuModule::SegmentBase::CreateSegment(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	assert(!m_vertCont, "Segment already created");

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();

	m_menu = menu;
	m_parent = parent;
	m_vertCont = uiRoot->AddComponent<UiModule::VertCont>(parent);
	m_menuController = menu->m_menuController;
}

void ModMenuModule::SegmentBase::DestroySegment()
{
	if(m_vertCont) {
		UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
		uiRoot->RemoveComponent(m_vertCont, true);
		m_vertCont = nullptr;
	}

	m_menu = nullptr;
	m_menuController = nullptr;
}

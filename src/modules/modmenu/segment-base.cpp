#include "segment-base.h"
#include "root.h"

ModMenuModule::SegmentBase::~SegmentBase()
{
	assert(!m_attached && "Segment must be detached before destruction");
}

void ModMenuModule::SegmentBase::SetAttached(bool attached, ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	if (m_attached == attached) return;
	if(attached) {
		assert(menu && parent && "Menu and parent must be provided when attaching");
		m_attached = Attach(menu, parent);
	} else {
		assert(!menu && !parent && "Menu and parent must be null when detaching");
		SetVisible(false);
		Detach();
		m_attached = false;
	}
}

void ModMenuModule::SegmentBase::SetVisible(bool visible)
{
	if (m_visible == visible) return;
	m_visible = visible;
	if (m_visible) {
		OnShow();
	} else {
		OnHide();
	}
}

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
	assert(!m_vertCont && "Segment already created");

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();

	m_menu = menu;
	m_parent = parent;
	m_vertCont = uiRoot->AddComponent<UiModule::VertCont>(parent);
	m_menuController = menu->m_menuController;
	m_menuGroupId = m_menuController->RegisterGroup();
	m_menuController->SetCurrentGroupId(m_menuGroupId);
}

void ModMenuModule::SegmentBase::DestroySegment()
{
	m_menuController->DeleteGroupItems(m_menuGroupId);

	if(m_vertCont) {
		UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
		uiRoot->RemoveComponent(m_vertCont, true);
		m_vertCont = nullptr;
	}

	m_menu = nullptr;
	m_menuController = nullptr;
	m_menuGroupId = -1;
}

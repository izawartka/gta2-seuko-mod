#include "shadows-fix.h"
#include "../../cheat-registry.h"

ModMenuModule::ShadowsFixCheat* ModMenuModule::ShadowsFixCheat::m_instance = nullptr;

ModMenuModule::ShadowsFixCheat::ShadowsFixCheat() : ModMenuModule::CheatBase("Cheat_ShadowsFix_IsEnabled") {
	assert(m_instance == nullptr && "ShadowsFixCheat instance already exists");
	m_instance = this;
}

ModMenuModule::ShadowsFixCheat::~ShadowsFixCheat()
{
	m_instance = nullptr;
}

ModMenuModule::ShadowsFixCheat* ModMenuModule::ShadowsFixCheat::GetInstance()
{
	assert(m_instance && "ShadowsFixCheat not initialized!");
	return m_instance;
}

void ModMenuModule::ShadowsFixCheat::OnFirstEnable()
{
}

void ModMenuModule::ShadowsFixCheat::OnEnable()
{
	AddEventListener<ScaleShadowEvent>(&ModMenuModule::ShadowsFixCheat::OnScaleShadow);
}

void ModMenuModule::ShadowsFixCheat::OnDisable()
{
	RemoveEventListener<ScaleShadowEvent>();
}

void ModMenuModule::ShadowsFixCheat::OnScaleShadow(ScaleShadowEvent& event)
{
	Game::Camera* mainCamera = Game::Memory::GetMainCamera();
	if (mainCamera == nullptr) return;

	float shadowsDistance = (1.0f / (Game::Utils::ToFloat(mainCamera->cameraPos.z) + 7.0f)) * 8.0f;
	float modifiedScale = static_cast<float>(event.GetScale()) * shadowsDistance;

	event.SetModifiedScale(static_cast<Game::SCR_f>(modifiedScale));
}

REGISTER_CHEAT(ShadowsFixCheat);

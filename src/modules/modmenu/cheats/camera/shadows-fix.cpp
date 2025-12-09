#include "shadows-fix.h"
#include "../../cheat-registry.h"

ModMenuModule::ShadowsFixCheat::ShadowsFixCheat() : ModMenuModule::CheatBase("Cheat_ShadowsFix_IsEnabled") {
}

ModMenuModule::ShadowsFixCheat::~ShadowsFixCheat()
{
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

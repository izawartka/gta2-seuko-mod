#pragma once
#include "../common.h"

namespace ModMenuModule::Utils::ScrfLerp {
    void Lerp(float& inout, float target, float t, float maxDelta);
    void Lerp(Game::SCR_f& inout, Game::SCR_f target, float t, Game::SCR_f maxDelta);
    void Lerp(Game::SCR_Vector3& inout, const Game::SCR_Vector3& target, float t, Game::SCR_f maxDelta);
    void Lerp(Game::CameraPos& inout, const Game::CameraPos& target, float t, Game::SCR_f maxDelta, int zoomMaxDelta);
    void LerpAsAngleMag(
        Game::SCR_Vector3& inout,
        const Game::SCR_Vector3& target,
        float magT,
        Game::SCR_f maxMagDelta,
        float horAngleT,
        float horAngleMaxDelta,
        float vertAngleT,
        float vertAngleMaxDelta
    );
}

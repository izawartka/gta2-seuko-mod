#include "scrf-lerp-utils.h"
#include "angle-utils.h"

void ModMenuModule::Utils::ScrfLerp::Lerp(float& inout, float target, float t, float maxDelta) {
    float difference = target - inout;
    float delta = std::clamp(difference * t, -maxDelta, maxDelta);
    inout += delta;
}

void ModMenuModule::Utils::ScrfLerp::Lerp(Game::SCR_f& inout, Game::SCR_f target, float t, Game::SCR_f maxDelta) {
    Game::SCR_f difference = target - inout;
    Game::SCR_f delta = std::clamp(static_cast<Game::SCR_f>(difference * t + 0.5f), -maxDelta, maxDelta);
    inout += delta;
}

void ModMenuModule::Utils::ScrfLerp::Lerp(Game::SCR_Vector3& inout, const Game::SCR_Vector3& target, float t, Game::SCR_f maxDelta) {
    Lerp(inout.x, target.x, t, maxDelta);
    Lerp(inout.y, target.y, t, maxDelta);
    Lerp(inout.z, target.z, t, maxDelta);
}

void ModMenuModule::Utils::ScrfLerp::Lerp(Game::CameraPos& inout, const Game::CameraPos& target, float t, Game::SCR_f maxDelta, int zoomMaxDelta) {
    Lerp(inout.x, target.x, t, maxDelta);
    Lerp(inout.y, target.y, t, maxDelta);
    Lerp(inout.z, target.z, t, maxDelta);
    Lerp(inout.zoom, target.zoom, t, zoomMaxDelta);
}

void ModMenuModule::Utils::ScrfLerp::LerpAsAngleMag(
    Game::SCR_Vector3& inout,
    const Game::SCR_Vector3& target,
    float magT,
    Game::SCR_f maxMagDelta,
    float horAngleT,
    float horAngleMaxDelta,
    float vertAngleT,
    float vertAngleMaxDelta
) {
    const float ix = Game::Utils::ToFloat(inout.x);
    const float iy = Game::Utils::ToFloat(inout.y);
    const float iz = Game::Utils::ToFloat(inout.z);
    const float tx = Game::Utils::ToFloat(target.x);
    const float ty = Game::Utils::ToFloat(target.y);
    const float tz = Game::Utils::ToFloat(target.z);

    float inMag = std::sqrt(ix * ix + iy * iy + iz * iz);
    float targetMag = std::sqrt(tx * tx + ty * ty + tz * tz);

    if (inMag == 0 && targetMag == 0) return;

    float inHorAngle = 0.0f;
    float inVertAngle = 0.0f;
    float targetHorAngle = 0.0f;
    float targetVertAngle = 0.0f;

    if (inMag > 0) {
        inHorAngle = std::atan2(iy, ix);
        inVertAngle = std::atan2(iz, std::sqrt(ix * ix + iy * iy));
    }
    if (targetMag > 0) {
        targetHorAngle = std::atan2(ty, tx);
        targetVertAngle = std::atan2(tz, std::sqrt(tx * tx + ty * ty));
    }

    if (inMag == 0 && targetMag > 0) {
        inHorAngle = targetHorAngle;
        inVertAngle = targetVertAngle;
    }
    else if (targetMag == 0 && inMag > 0) {
        targetHorAngle = inHorAngle;
        targetVertAngle = inVertAngle;
    }

    const float newHorAngle = ModMenuModule::Utils::Angle::LerpAngleWithLimit(
        inHorAngle, targetHorAngle, horAngleT, horAngleMaxDelta);

    const float newVertAngle = ModMenuModule::Utils::Angle::LerpAngleWithLimit(
        inVertAngle, targetVertAngle, vertAngleT, vertAngleMaxDelta);

    float newMag = inMag;
    Lerp(newMag, targetMag, magT, Game::Utils::ToFloat(maxMagDelta));

    const float cosVert = std::cos(newVertAngle);
    inout.x = Game::Utils::FromFloat(newMag * cosVert * std::cos(newHorAngle));
    inout.y = Game::Utils::FromFloat(newMag * cosVert * std::sin(newHorAngle));
    inout.z = Game::Utils::FromFloat(newMag * std::sin(newVertAngle));
}

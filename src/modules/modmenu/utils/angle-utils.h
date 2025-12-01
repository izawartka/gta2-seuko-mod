#pragma once
#include "../common.h"

namespace ModMenuModule::Utils::Angle {
	float NormalizeAngle(float angleRad);
	float GetShortestAngleDifference(float fromAngleRad, float toAngleRad);
	float LerpAngle(float fromAngleRad, float toAngleRad, float t);
}

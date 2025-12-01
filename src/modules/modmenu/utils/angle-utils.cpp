#include "angle-utils.h"

float ModMenuModule::Utils::Angle::NormalizeAngle(float angleRad) {
	std::fesetround(FE_TONEAREST);
	constexpr float twoPi = static_cast<float>(2.0 * M_PI);

	if(angleRad < 0) {
		return twoPi + fmodf(angleRad, twoPi);
	}

	if(angleRad >= twoPi) {
		return fmodf(angleRad, twoPi);
	}

	return angleRad;
}

float ModMenuModule::Utils::Angle::GetShortestAngleDifference(float fromAngleRad, float toAngleRad) {
	std::fesetround(FE_TONEAREST);
	const float normalizedFrom = NormalizeAngle(fromAngleRad);
	const float normalizedTo = NormalizeAngle(toAngleRad);
	float difference = normalizedTo - normalizedFrom;
	if(difference > static_cast<float>(M_PI)) {
		difference -= static_cast<float>(2.0 * M_PI);
	}
	else if(difference < -static_cast<float>(M_PI)) {
		difference += static_cast<float>(2.0 * M_PI);
	}
	return difference;
}

float ModMenuModule::Utils::Angle::LerpAngle(float fromAngleRad, float toAngleRad, float t) {
	std::fesetround(FE_TONEAREST);
	float difference = GetShortestAngleDifference(fromAngleRad, toAngleRad);
	return NormalizeAngle(fromAngleRad + difference * t);
}

#pragma once
#include "../common.h"

namespace ModMenuModule::Utils::Vertex {
	struct CameraValues {
		std::optional<Game::GTAVertex> pedOffsetVertex;
		float xCenter;
		float yCenter;
		float field60;
		float gameCameraX;
		float gameCameraY;
		float gameCameraZ;
		float horRotCenter;
		float zOffset;
	};

	struct CustomCameraPos {
		float x;
		float y;
		float z;
	};

	struct CameraTransform {
		float horizontalAngleRad = 0.0f;
		float verticalAngleRad = 0.0f;
		float additionalZOffset = 0.0f;
		float arrowsScale = 1.0f;

		bool operator==(const CameraTransform& other) const {
			return horizontalAngleRad == other.horizontalAngleRad &&
				verticalAngleRad == other.verticalAngleRad &&
				additionalZOffset == other.additionalZOffset &&
				arrowsScale == other.arrowsScale;
		}

		bool operator!=(const CameraTransform& other) const {
			return !(*this == other);
		}
	};

	struct CachedCameraTransform {
		float horizontalAngleSin = 0.0f;
		float horizontalAngleCos = 1.0f;
		float verticalAngleSin = 0.0f;
		float verticalAngleCos = 1.0f;
		float additionalZOffset = 0.0f;
		float arrowsScale = 1.0f;
		bool needsVerticalRotation = false;
		bool needsHorizontalRotation = false;
		bool needsWorldSpaceTransform = false;
		bool needsArrowsWorldSpaceTransform = false;

		CachedCameraTransform() = default;
		CachedCameraTransform(const CameraTransform& cameraTransform) {
			horizontalAngleSin = sinf(cameraTransform.horizontalAngleRad);
			horizontalAngleCos = cosf(cameraTransform.horizontalAngleRad);
			verticalAngleSin = sinf(cameraTransform.verticalAngleRad);
			verticalAngleCos = cosf(cameraTransform.verticalAngleRad);
			additionalZOffset = cameraTransform.additionalZOffset;
			arrowsScale = cameraTransform.arrowsScale;
			needsVerticalRotation = cameraTransform.verticalAngleRad != 0.0f;
			needsHorizontalRotation = cameraTransform.horizontalAngleRad != 0.0f;
			needsWorldSpaceTransform = needsHorizontalRotation || cameraTransform.additionalZOffset != 0.0f;
			needsArrowsWorldSpaceTransform = needsWorldSpaceTransform || cameraTransform.arrowsScale != 1.0f;
		}
	};

	void ToCenteredScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues);
	void FromCenteredScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues);
	void RotateVertexX(Game::GTAVertex& vertex, float angleSin, float angleCos, float centerZ);
	void RotateVertexZ(Game::GTAVertex& vertex, float angleSin, float angleCos);
	void ToWorldSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues);
	void ToScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues);
	float GetCrossProduct(const Game::GTAVertex& v1, const Game::GTAVertex& v2, const Game::GTAVertex& v3);
	std::optional<Game::GTAVertex> GetPedOffsetVertex(const Game::Camera& viewCamera, const CachedCameraTransform& cachedCameraTransform);
	void TransformArrows(Game::GTAVertex& vertex, const CameraValues& cameraValues, const CachedCameraTransform& cachedCameraTransform);
	
	void ApplyQuadCameraTransform(Game::GTAVertex* vertices, const CameraValues& cameraValues, const CachedCameraTransform& cachedCameraTransform);
	void ApplyArrowsCameraTransform(Game::GTAVertex* vertices, const CameraValues& cameraValues, const CachedCameraTransform& cachedCameraTransform);
	void ApplyTriangleCameraTransform(Game::GTAVertex* vertices, const CameraValues& cameraValues, const CachedCameraTransform& cachedCameraTransform, bool* isReversedOut = nullptr);
	bool ApplyCustomCulling(Game::GTAVertex* vertices, size_t vertexCount, const CameraValues& cameraValues, bool isReversed = false);

	CameraValues GetCameraValues(const Game::Camera& camera, const CachedCameraTransform& cachedCameraTransform);
	CustomCameraPos GetCustomCameraPos(const CameraValues& cameraValues, const CachedCameraTransform& cachedCameraTransform);
}

#pragma once
#include "../common.h"

namespace ModMenuModule::Utils::Vertex {
	struct CameraValues {
		float xCenter;
		float yCenter;
		float zCenter;
		float horZCenter;
		float zoomFactor;
		float field60;
		float gameCameraX;
		float gameCameraY;
		float gameCameraZ;
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
	};

	void ToCenteredScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues);
	void FromCenteredScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues);
	void RotateVertexX(Game::GTAVertex& vertex, float angleRad);
	void RotateVertexZ(Game::GTAVertex& vertex, float angleRad);
	void ToWorldSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues);
	void ToScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues);
	float GetCrossProduct(const Game::GTAVertex& v1, const Game::GTAVertex& v2, const Game::GTAVertex& v3);
	bool DoReverseTriangleCross(const Game::GTAVertex* vertices, const CameraValues& cameraValues);
	
	void ApplyCameraTransform(Game::GTAVertex* vertices, size_t vertexCount, const CameraValues& cameraValues, const CameraTransform& cameraTransform);
	bool ApplyCustomCulling(Game::GTAVertex* vertices, size_t vertexCount, const CameraValues& cameraValues);

	CameraValues GetCameraValues(const Game::Camera& camera, const Game::Ped* playerPed);
	CustomCameraPos GetCustomCameraPos(const CameraValues& cameraValues, const CameraTransform& cameraTransform);
}

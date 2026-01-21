#include "vertex-utils.h"

void ModMenuModule::Utils::Vertex::ToCenteredScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues)
{
	vertex.x -= cameraValues.xCenter;
	vertex.y -= cameraValues.yCenter;
	vertex.z -= cameraValues.zCenter;
}

void ModMenuModule::Utils::Vertex::FromCenteredScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues)
{
	vertex.x += cameraValues.xCenter;
	vertex.y += cameraValues.yCenter;
	vertex.z += cameraValues.zCenter;
}

void ModMenuModule::Utils::Vertex::RotateVertexX(Game::GTAVertex& vertex, float angleRad)
{
	float cosAngle = cosf(angleRad);
	float sinAngle = sinf(angleRad);

	float y = vertex.y * cosAngle - vertex.z * sinAngle;
	float z = vertex.y * sinAngle + vertex.z * cosAngle;

	vertex.y = y;
	vertex.z = z;
}

void ModMenuModule::Utils::Vertex::RotateVertexZ(Game::GTAVertex& vertex, float angleRad)
{
	float cosAngle = cosf(angleRad);
	float sinAngle = sinf(angleRad);

	float x = vertex.x * cosAngle - vertex.y * sinAngle;
	float y = vertex.x * sinAngle + vertex.y * cosAngle;

	vertex.x = x;
	vertex.y = y;
}

void ModMenuModule::Utils::Vertex::ToWorldSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues)
{
	vertex.x /= (cameraValues.field60 * vertex.z);
	vertex.y /= (cameraValues.field60 * vertex.z);
	vertex.z = cameraValues.perspFactor - (1.0f / vertex.z);
	vertex.z -= cameraValues.zOffset;
}

void ModMenuModule::Utils::Vertex::ToScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues)
{
	vertex.z += cameraValues.zOffset;
	vertex.z = 1.0f / (cameraValues.perspFactor - vertex.z);
	vertex.x *= cameraValues.field60 * vertex.z;
	vertex.y *= cameraValues.field60 * vertex.z;
}

void ModMenuModule::Utils::Vertex::ToSetZPosition(Game::GTAVertex& vertex, float newZ, const CameraValues& cameraValues)
{
	float dFactor = cameraValues.perspFactor - cameraValues.zOffset;
	vertex.x *= (dFactor - newZ) / (dFactor - vertex.z);
	vertex.y *= (dFactor - newZ) / (dFactor - vertex.z);
	vertex.z = newZ;
}

void ModMenuModule::Utils::Vertex::ScaleXY(Game::GTAVertex& vertex, float scale)
{
	vertex.x *= scale;
	vertex.y *= scale;
}

float ModMenuModule::Utils::Vertex::GetCrossProduct(const Game::GTAVertex& v1, const Game::GTAVertex& v2, const Game::GTAVertex& v3)
{
	float ax = v2.x - v1.x;
	float ay = v2.y - v1.y;
	float bx = v3.x - v1.x;
	float by = v3.y - v1.y;
	return ax * by - ay * bx;
}

void ModMenuModule::Utils::Vertex::ApplyQuadCameraTransform(Game::GTAVertex* vertices, const CameraValues& cameraValues, const CameraTransform& cameraTransform)
{
	for (size_t i = 0; i < 4; ++i) {
		ToCenteredScreenSpaceVertex(vertices[i], cameraValues);
		Game::GTAVertex& vertex = vertices[i];
		if(cameraTransform.verticalAngleRad != 0.0f) {
			RotateVertexZ(vertex, cameraTransform.verticalAngleRad);
		}
		if(cameraTransform.horizontalAngleRad != 0.0f || cameraTransform.additionalZOffset) {
			ToWorldSpaceVertex(vertex, cameraValues);
			if (cameraTransform.horizontalAngleRad != 0.0f) {
				RotateVertexX(vertex, cameraTransform.horizontalAngleRad);
			}
			vertex.z += cameraTransform.additionalZOffset;
			ToScreenSpaceVertex(vertex, cameraValues);
		}
		FromCenteredScreenSpaceVertex(vertex, cameraValues);
	}
}

void ModMenuModule::Utils::Vertex::ApplyArrowsCameraTransform(Game::GTAVertex* vertices, const CameraValues& cameraValues, const CameraTransform& cameraTransform)
{
	for (size_t i = 0; i < 4; ++i) {
		ToCenteredScreenSpaceVertex(vertices[i], cameraValues);
		Game::GTAVertex& vertex = vertices[i];
		if (cameraTransform.verticalAngleRad != 0.0f) {
			RotateVertexZ(vertex, cameraTransform.verticalAngleRad);
		}
		if (cameraTransform.horizontalAngleRad != 0.0f || cameraTransform.arrowsScale != 1.0f) {
			ToWorldSpaceVertex(vertex, cameraValues);
			ToSetZPosition(vertex, -0.5f, cameraValues);
			ScaleXY(vertex, cameraTransform.arrowsScale);
			if (cameraTransform.horizontalAngleRad != 0.0f) {
				RotateVertexX(vertex, cameraTransform.horizontalAngleRad);
			}
			ToScreenSpaceVertex(vertex, cameraValues);
		}
		FromCenteredScreenSpaceVertex(vertex, cameraValues);
	}
}

void ModMenuModule::Utils::Vertex::ApplyTriangleCameraTransform(Game::GTAVertex* vertices, const CameraValues& cameraValues, const CameraTransform& cameraTransform, bool* isReversedOut)
{
	Game::GTAVertex worldSpaceVerts[3];

	for (size_t i = 0; i < 3; ++i) {
		ToCenteredScreenSpaceVertex(vertices[i], cameraValues);
		Game::GTAVertex& vertex = vertices[i];
		if(cameraTransform.verticalAngleRad != 0.0f) {
			RotateVertexZ(vertex, cameraTransform.verticalAngleRad);
		}
		if (cameraTransform.horizontalAngleRad != 0.0f || cameraTransform.additionalZOffset || isReversedOut != nullptr) {
			ToWorldSpaceVertex(vertex, cameraValues);
			worldSpaceVerts[i] = vertex;
			if(cameraTransform.horizontalAngleRad != 0.0f) {
				RotateVertexX(vertex, cameraTransform.horizontalAngleRad);
			}
			vertex.z += cameraTransform.additionalZOffset;
			ToScreenSpaceVertex(vertex, cameraValues);
		}
		FromCenteredScreenSpaceVertex(vertex, cameraValues);
	}

	if (isReversedOut != nullptr) {
		float cross = GetCrossProduct(worldSpaceVerts[0], worldSpaceVerts[1], worldSpaceVerts[2]);
		*isReversedOut = cross < 0.0f;
	}
}

bool ModMenuModule::Utils::Vertex::ApplyCustomCulling(Game::GTAVertex* vertices, size_t vertexCount, const CameraValues& cameraValues, bool isReversed)
{
	bool culled = false;

	// backface culling
	float cross = isReversed ? GetCrossProduct(vertices[0], vertices[2], vertices[1]) : GetCrossProduct(vertices[0], vertices[1], vertices[2]);
	if (cross < 0.0f) {
		culled = true;
	}

	// frustum culling
	if (!culled) {
		size_t outsideTopCount = 0;
		size_t outsideBottomCount = 0;
		size_t outsideLeftCount = 0;
		size_t outsideRightCount = 0;

		for (size_t i = 0; i < vertexCount; i++) {
			if (vertices[i].z < 0.0f) {
				culled = true;
				break;
			}
			if (vertices[i].y < 0.0f) outsideTopCount++;
			if (vertices[i].y > cameraValues.yCenter * 2) outsideBottomCount++;
			if (vertices[i].x < 0.0f) outsideLeftCount++;
			if (vertices[i].x > cameraValues.xCenter * 2) outsideRightCount++;
		}

		if (outsideTopCount == vertexCount ||
			outsideBottomCount == vertexCount ||
			outsideLeftCount == vertexCount ||
			outsideRightCount == vertexCount) {
			culled = true;
		}
	}

	if (!culled) return false;

	// apply culling
	for (size_t i = 0; i < vertexCount; i++) {
		vertices[i].x = 0.0f;
		vertices[i].y = 0.0f;
		vertices[i].z = 0.0f;
	}

	return true;
}

ModMenuModule::Utils::Vertex::CameraValues ModMenuModule::Utils::Vertex::GetCameraValues(const Game::Camera& camera, Game::SCR_f playerPedZ)
{
	const int screenWidth = *Game::Memory::GetScreenWidth();
	const int screenHeight = *Game::Memory::GetScreenHeight();

	float xCenter = static_cast<float>(screenWidth) / 2.0f;
	float yCenter = static_cast<float>(screenHeight) / 2.0f;
	float zCenter = 0;
	float horZCenter = Game::Utils::ToFloat(playerPedZ) + 0.5f;
	float zoomFactor = static_cast<float>(camera.cameraPos.zoom) / 14540.0f;
	float field60 = Game::Utils::ToFloat(camera.field_0x60);
	float gameCameraX = Game::Utils::ToFloat(camera.cameraPos.x);
	float gameCameraY = Game::Utils::ToFloat(camera.cameraPos.y);
	float gameCameraZ = Game::Utils::ToFloat(camera.cameraPos.z);
	float zOffset = horZCenter + 8.0f * (zoomFactor - 1.0f);
	float perspFactor = gameCameraZ + 8.0f * zoomFactor;

	return { xCenter, yCenter, zCenter, horZCenter, zoomFactor, field60, gameCameraX, gameCameraY, gameCameraZ, zOffset, perspFactor };
}

ModMenuModule::Utils::Vertex::CustomCameraPos ModMenuModule::Utils::Vertex::GetCustomCameraPos(const CameraValues& cameraValues, const CameraTransform& cameraTransform)
{
	float baseX = cameraValues.gameCameraX;
	float baseY = cameraValues.gameCameraY;
	float baseZ = cameraValues.gameCameraZ + 8.0f - cameraTransform.additionalZOffset;

	float horizontalY = sin(cameraTransform.horizontalAngleRad) * (baseZ - cameraValues.horZCenter);
	float horizontalZ = cos(cameraTransform.horizontalAngleRad) * (baseZ - cameraValues.horZCenter);

	float verticalX = horizontalY * sin(cameraTransform.verticalAngleRad);
	float verticalY = horizontalY * cos(cameraTransform.verticalAngleRad);

	float customCameraX = baseX + verticalX;
	float customCameraY = baseY + verticalY;
	float customCameraZ = cameraValues.horZCenter + horizontalZ;

	return { customCameraX, customCameraY, customCameraZ };
}

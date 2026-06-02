#include "vertex-utils.h"
#include "get-ped-position.h"

static constexpr float ARROWS_NEW_Z = 0.25f;

void ModMenuModule::Utils::Vertex::ToCenteredScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues)
{
	vertex.x -= cameraValues.xCenter;
	vertex.y -= cameraValues.yCenter;
}

void ModMenuModule::Utils::Vertex::FromCenteredScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues)
{
	vertex.x += cameraValues.xCenter;
	vertex.y += cameraValues.yCenter;
}

void ModMenuModule::Utils::Vertex::RotateVertexX(Game::GTAVertex& vertex, float angleSin, float angleCos, float centerZ)
{
	vertex.z -= centerZ;

	float y = vertex.y * angleCos - vertex.z * angleSin;
	float z = vertex.y * angleSin + vertex.z * angleCos;

	vertex.y = y;
	vertex.z = z + centerZ;
}

void ModMenuModule::Utils::Vertex::RotateVertexZ(Game::GTAVertex& vertex, float angleSin, float angleCos)
{
	float x = vertex.x * angleCos - vertex.y * angleSin;
	float y = vertex.x * angleSin + vertex.y * angleCos;

	vertex.x = x;
	vertex.y = y;
}

void ModMenuModule::Utils::Vertex::ToWorldSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues)
{
	vertex.x /= (cameraValues.field60 * vertex.z);
	vertex.y /= (cameraValues.field60 * vertex.z);
	vertex.z = cameraValues.zOffset - (1.0f / vertex.z);
}

void ModMenuModule::Utils::Vertex::ToScreenSpaceVertex(Game::GTAVertex& vertex, const CameraValues& cameraValues)
{
	vertex.z = 1.0f / (cameraValues.zOffset - vertex.z);
	vertex.x *= cameraValues.field60 * vertex.z;
	vertex.y *= cameraValues.field60 * vertex.z;
}

float ModMenuModule::Utils::Vertex::GetCrossProduct(const Game::GTAVertex& v1, const Game::GTAVertex& v2, const Game::GTAVertex& v3)
{
	float ax = v2.x - v1.x;
	float ay = v2.y - v1.y;
	float bx = v3.x - v1.x;
	float by = v3.y - v1.y;
	return ax * by - ay * bx;
}

std::optional<Game::GTAVertex> ModMenuModule::Utils::Vertex::GetPedOffsetVertex(
	const Game::Camera& viewCamera,
	const CachedCameraTransform& cachedCameraTransform
)
{
	const Game::SCR_Vector3* pedPosition = Utils::GetPedPosition(viewCamera.ped);
	if (!pedPosition) {
		return std::nullopt;
	}

	float x = Game::Utils::ToFloat(pedPosition->x - viewCamera.cameraPos.x);
	float y = Game::Utils::ToFloat(pedPosition->y - viewCamera.cameraPos.y);
	float z = Game::Utils::ToFloat(pedPosition->z);

	float x2 = x;
	float y2 = y;

	if(cachedCameraTransform.needsVerticalRotation) {
		x2 = x * cachedCameraTransform.verticalAngleCos - y * cachedCameraTransform.verticalAngleSin;
		y2 = x * cachedCameraTransform.verticalAngleSin + y * cachedCameraTransform.verticalAngleCos;
	}

	return Game::GTAVertex{
		x2,
		y2,
		z
	};
}

void ModMenuModule::Utils::Vertex::TransformArrows(
	Game::GTAVertex& vertex, 
	const CameraValues& cameraValues,
	const CachedCameraTransform& cachedCameraTransform
)
{
	if (!cameraValues.pedOffsetVertex.has_value()) return;

	float targetZ = cameraValues.pedOffsetVertex->z + ARROWS_NEW_Z;
	float newZScale = (cameraValues.zOffset - targetZ) / (cameraValues.zOffset - vertex.z);
	float addZScale = 1.0f - (cachedCameraTransform.additionalZOffset / (cameraValues.zOffset - targetZ));
	float centeredScale = cachedCameraTransform.arrowsScale * addZScale;

	vertex.x = (vertex.x * newZScale - cameraValues.pedOffsetVertex->x) * centeredScale + cameraValues.pedOffsetVertex->x;
	vertex.y = (vertex.y * newZScale - cameraValues.pedOffsetVertex->y) * centeredScale + cameraValues.pedOffsetVertex->y;
	vertex.z = targetZ;
}

void ModMenuModule::Utils::Vertex::ApplyQuadCameraTransform(
	Game::GTAVertex* vertices, 
	const CameraValues& cameraValues, 
	const CachedCameraTransform& cachedCameraTransform
) {
	for (size_t i = 0; i < 4; ++i) {
		ToCenteredScreenSpaceVertex(vertices[i], cameraValues);
		Game::GTAVertex& vertex = vertices[i];
		if (cachedCameraTransform.needsVerticalRotation) {
			RotateVertexZ(vertex, cachedCameraTransform.verticalAngleSin, cachedCameraTransform.verticalAngleCos);
		}
		if (cachedCameraTransform.needsWorldSpaceTransform) {
			ToWorldSpaceVertex(vertex, cameraValues);
			if (cachedCameraTransform.needsHorizontalRotation) {
				RotateVertexX(vertex, cachedCameraTransform.horizontalAngleSin, cachedCameraTransform.horizontalAngleCos, cachedCameraTransform.horRotCenter);
			}
			vertex.z += cachedCameraTransform.additionalZOffset;
			ToScreenSpaceVertex(vertex, cameraValues);
		}
		FromCenteredScreenSpaceVertex(vertex, cameraValues);
	}
}

void ModMenuModule::Utils::Vertex::ApplyArrowsCameraTransform(
	Game::GTAVertex* vertices, 
	const CameraValues& cameraValues,
	const CachedCameraTransform& cachedCameraTransform
) {
	for (size_t i = 0; i < 4; ++i) {
		ToCenteredScreenSpaceVertex(vertices[i], cameraValues);
		Game::GTAVertex& vertex = vertices[i];
		if (cachedCameraTransform.needsVerticalRotation) {
			RotateVertexZ(vertex, cachedCameraTransform.verticalAngleSin, cachedCameraTransform.verticalAngleCos);
		}
		if (cachedCameraTransform.needsArrowsWorldSpaceTransform) {
			ToWorldSpaceVertex(vertex, cameraValues);
			TransformArrows(vertex, cameraValues, cachedCameraTransform);
			if (cachedCameraTransform.needsHorizontalRotation != 0.0f) {
				RotateVertexX(vertex, cachedCameraTransform.horizontalAngleSin, cachedCameraTransform.horizontalAngleCos, cachedCameraTransform.horRotCenter);
			}
			vertex.z += cachedCameraTransform.additionalZOffset;
			ToScreenSpaceVertex(vertex, cameraValues);
		}
		FromCenteredScreenSpaceVertex(vertex, cameraValues);
	}
}

void ModMenuModule::Utils::Vertex::ApplyTriangleCameraTransform(
	Game::GTAVertex* vertices, 
	const CameraValues& cameraValues, 
	const CachedCameraTransform& cachedCameraTransform, 
	bool* isReversedOut
) {
	Game::GTAVertex worldSpaceVerts[3];

	for (size_t i = 0; i < 3; ++i) {
		ToCenteredScreenSpaceVertex(vertices[i], cameraValues);
		Game::GTAVertex& vertex = vertices[i];
		if (cachedCameraTransform.needsVerticalRotation) {
			RotateVertexZ(vertex, cachedCameraTransform.verticalAngleSin, cachedCameraTransform.verticalAngleCos);
		}
		if (cachedCameraTransform.needsWorldSpaceTransform || isReversedOut != nullptr) {
			ToWorldSpaceVertex(vertex, cameraValues);
			worldSpaceVerts[i] = vertex;
			if (cachedCameraTransform.needsHorizontalRotation) {
				RotateVertexX(vertex, cachedCameraTransform.horizontalAngleSin, cachedCameraTransform.horizontalAngleCos, cachedCameraTransform.horRotCenter);
			}
			vertex.z += cachedCameraTransform.additionalZOffset;
			ToScreenSpaceVertex(vertex, cameraValues);
		}
		FromCenteredScreenSpaceVertex(vertex, cameraValues);
	}

	if (isReversedOut != nullptr) {
		float cross = GetCrossProduct(worldSpaceVerts[0], worldSpaceVerts[1], worldSpaceVerts[2]);
		*isReversedOut = cross < 0.0f;
	}
}

void ModMenuModule::Utils::Vertex::ApplyWorldPointCameraTransform(
	Game::SCR_Vector3& point, 
	const CameraValues& cameraValues, 
	const CachedCameraTransform& cachedCameraTransform
) {
	// unusual transform: we're working on absolute fixed point world coords

	Game::GTAVertex vertex = {
		Game::Utils::ToFloat(point.x) - cameraValues.gameCameraX,
		Game::Utils::ToFloat(point.y) - cameraValues.gameCameraY,
		Game::Utils::ToFloat(point.z)
	};

	if (cachedCameraTransform.needsVerticalRotation) {
		RotateVertexZ(vertex, cachedCameraTransform.verticalAngleSin, cachedCameraTransform.verticalAngleCos);
	}

	if (cachedCameraTransform.needsHorizontalRotation) {
		RotateVertexX(vertex, cachedCameraTransform.horizontalAngleSin, cachedCameraTransform.horizontalAngleCos, cachedCameraTransform.horRotCenter);
	}
	vertex.z += cachedCameraTransform.additionalZOffset;

	point.x = Game::Utils::FromFloat(vertex.x + cameraValues.gameCameraX);
	point.y = Game::Utils::FromFloat(vertex.y + cameraValues.gameCameraY);
	point.z = Game::Utils::FromFloat(vertex.z);
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

ModMenuModule::Utils::Vertex::CameraValues ModMenuModule::Utils::Vertex::GetCameraValues(const Game::Camera& camera, const CachedCameraTransform& cachedCameraTransform)
{
	const int screenWidth = *Game::Memory::GetScreenWidth();
	const int screenHeight = *Game::Memory::GetScreenHeight();

	auto pedOffsetVertex = GetPedOffsetVertex(camera, cachedCameraTransform);
	float xCenter = static_cast<float>(screenWidth) / 2.0f;
	float yCenter = static_cast<float>(screenHeight) / 2.0f;
	float field60 = Game::Utils::ToFloat(camera.field_0x60);
	float gameCameraX = Game::Utils::ToFloat(camera.cameraPos.x);
	float gameCameraY = Game::Utils::ToFloat(camera.cameraPos.y);
	float gameCameraZ = Game::Utils::ToFloat(camera.cameraPos.z);
	float zOffset = gameCameraZ + 8.0f;

	return { pedOffsetVertex, xCenter, yCenter, field60, gameCameraX, gameCameraY, gameCameraZ, zOffset };
}

ModMenuModule::Utils::Vertex::CustomCameraPos ModMenuModule::Utils::Vertex::GetCustomCameraPos(
	const CameraValues& cameraValues, 
	const CachedCameraTransform& cachedCameraTransform
) {
	float baseX = cameraValues.gameCameraX;
	float baseY = cameraValues.gameCameraY;
	float baseZ = cameraValues.gameCameraZ + 8.0f - cachedCameraTransform.additionalZOffset;

	float horizontalY = cachedCameraTransform.horizontalAngleSin * (baseZ - cachedCameraTransform.horRotCenter);
	float horizontalZ = cachedCameraTransform.horizontalAngleCos * (baseZ - cachedCameraTransform.horRotCenter);

	float verticalX = horizontalY * cachedCameraTransform.verticalAngleSin;
	float verticalY = horizontalY * cachedCameraTransform.verticalAngleCos;

	float customCameraX = baseX + verticalX;
	float customCameraY = baseY + verticalY;
	float customCameraZ = cachedCameraTransform.horRotCenter + horizontalZ;

	return { customCameraX, customCameraY, customCameraZ };
}

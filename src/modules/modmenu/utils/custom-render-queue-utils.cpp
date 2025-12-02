#include "custom-render-queue-utils.h"

bool ModMenuModule::Utils::CustomRenderQueue::SafeAddToQueue(const QueueData& data, std::vector<std::array<int, 2>>& renderQueue, int x, int y)
{
	if (abs(x - data.gcX) > static_cast<int>(data.renderDistance)) return false;
	if (abs(y - data.gcY) > static_cast<int>(data.renderDistance)) return false;
	renderQueue.push_back({ x, y });
	return true;
}

std::vector<std::array<int, 2>> ModMenuModule::Utils::CustomRenderQueue::BuildRenderQueue(const QueueData& data)
{
	std::vector<std::array<int, 2>> renderQueue;
	renderQueue.reserve(data.queueSize);

	for (int r = data.maxRad; r >= 1; r--) {
		for (int i = r; i >= 0; i--) {
			if (i != r && i != 0) {
				SafeAddToQueue(data, renderQueue, data.ccX - i, data.ccY - r);
				SafeAddToQueue(data, renderQueue, data.ccX + r, data.ccY - i);
				SafeAddToQueue(data, renderQueue, data.ccX + i, data.ccY + r);
				SafeAddToQueue(data, renderQueue, data.ccX - r, data.ccY + i);
			}
			SafeAddToQueue(data, renderQueue, data.ccX + i, data.ccY - r);
			SafeAddToQueue(data, renderQueue, data.ccX + r, data.ccY + i);
			SafeAddToQueue(data, renderQueue, data.ccX - i, data.ccY + r);
			SafeAddToQueue(data, renderQueue, data.ccX - r, data.ccY - i);
		}
	}

	SafeAddToQueue(data, renderQueue, data.ccX, data.ccY);

	return renderQueue;
}

ModMenuModule::Utils::CustomRenderQueue::QueueData ModMenuModule::Utils::CustomRenderQueue::GetQueueData(
	const Utils::Vertex::CameraValues& cameraValues, 
	const Utils::Vertex::CustomCameraPos& customCameraPos, 
	size_t renderDistance
) {
	int gcX = static_cast<int>(cameraValues.gameCameraX);
	int gcY = static_cast<int>(cameraValues.gameCameraY);
	int ccX = static_cast<int>(customCameraPos.x);
	int ccY = static_cast<int>(customCameraPos.y);
	size_t queueSize = (2 * renderDistance + 1) * (2 * renderDistance + 1);
	int maxRadX = abs(gcX - ccX) + static_cast<int>(renderDistance);
	int maxRadY = abs(gcY - ccY) + static_cast<int>(renderDistance);
	int maxRad = std::max(maxRadX, maxRadY);

	return {
		static_cast<int>(customCameraPos.x),
		static_cast<int>(customCameraPos.y),
		static_cast<int>(cameraValues.gameCameraX),
		static_cast<int>(cameraValues.gameCameraY),
		renderDistance,
		queueSize,
		maxRad
	};
}

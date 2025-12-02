#pragma once
#include "vertex-utils.h"

namespace ModMenuModule::Utils::CustomRenderQueue {
	struct QueueData {
		int ccX;
		int ccY;
		int gcX;
		int gcY;
		size_t renderDistance;
		size_t queueSize;
		int maxRad;
	};

	bool SafeAddToQueue(
		const QueueData& data, 
		std::vector<std::array<int, 2>>& renderQueue, 
		int x, 
		int y
	);

	std::vector<std::array<int, 2>> BuildRenderQueue(const QueueData& data);

	QueueData GetQueueData(
		const Utils::Vertex::CameraValues& cameraValues, 
		const Utils::Vertex::CustomCameraPos& customCameraPos,
		size_t renderDistance
	);
}

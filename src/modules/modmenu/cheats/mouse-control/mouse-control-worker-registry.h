#pragma once
#include "../../common.h"
#include "mouse-control-worker.h"

namespace ModMenuModule::MouseControlWorkerRegistry {
	enum class WorkerSetType {
		None = 0,
		AttackOnly,
		RotateMode,
		RotateModeInCar,
		RotateModeAltMoving,
		RotateModeFreecam,
		PointAtMode,
	};

	using WorkerType = std::type_index;

	template<typename WorkerT>
	static inline WorkerType GetWorkerType()
	{
		return std::type_index(typeid(WorkerT));
	}

	static inline WorkerType GetWorkerType(MouseControlWorker* worker)
	{
		return worker ? std::type_index(typeid(*worker)) : std::type_index(typeid(void));
	}

	struct MouseControlWorkerSetDef {
		WorkerType attackWorkerType;
		WorkerType mouseWorkerType;
		WorkerType resultWorkerType;

		template<typename AttackWorkerT, typename MouseWorkerT, typename ResultWorkerT>
		static constexpr MouseControlWorkerSetDef Create()
		{
			return {
				GetWorkerType<AttackWorkerT>(),
				GetWorkerType<MouseWorkerT>(),
				GetWorkerType<ResultWorkerT>()
			};
		}
	};

	const MouseControlWorkerSetDef& GetWorkerSetDef(WorkerSetType type);
	const std::unique_ptr<MouseControlWorker> CreateWorker(WorkerType type);
}

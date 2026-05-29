#include "mouse-control-worker-registry.h"
#include "mouse-control-worker.h"
#include "attack-worker.h"
#include "locked-mouse-worker.h"
#include "point-at-mouse-worker.h"
#include "left-right-worker.h"
#include "rotate-camera-worker.h"

namespace ModMenuModule::MouseControlWorkerRegistry {
	namespace {
		using WorkerFactory = std::unique_ptr<MouseControlWorker>(*)();

		template<typename WorkerT>
		std::unique_ptr<MouseControlWorker> CreateWorkerInstance()
		{
			return std::make_unique<WorkerT>();
		}

		const std::unordered_map<WorkerType, const WorkerFactory>& GetWorkerFactories()
		{
			static const std::unordered_map<WorkerType, const WorkerFactory> factories = {
				{ GetWorkerType<AttackWorker>(), &CreateWorkerInstance<AttackWorker> },
				{ GetWorkerType<LockedMouseWorker>(), &CreateWorkerInstance<LockedMouseWorker> },
				{ GetWorkerType<PointAtMouseWorker>(), &CreateWorkerInstance<PointAtMouseWorker> },
				{ GetWorkerType<LeftRightWorker>(), &CreateWorkerInstance<LeftRightWorker> },
				{ GetWorkerType<RotateCameraWorker>(), &CreateWorkerInstance<RotateCameraWorker> },
			};

			return factories;
		}

		const std::unordered_map<WorkerSetType, const MouseControlWorkerSetDef>& GetWorkerSetDefs()
		{
			static const std::unordered_map<WorkerSetType, const MouseControlWorkerSetDef> workerSetDefs = {
				{ WorkerSetType::AttackOnly, MouseControlWorkerSetDef::Create<AttackWorker, void, void>() },
				{ WorkerSetType::RotateMode, MouseControlWorkerSetDef::Create<AttackWorker, LockedMouseWorker, LeftRightWorker>() },
				{ WorkerSetType::RotateModeInCar, MouseControlWorkerSetDef::Create<AttackWorker, LockedMouseWorker, RotateCameraWorker>() },
				{ WorkerSetType::RotateModeAltMoving, MouseControlWorkerSetDef::Create<AttackWorker, LockedMouseWorker, RotateCameraWorker>() },
				{ WorkerSetType::RotateModeFreecam, MouseControlWorkerSetDef::Create<void, LockedMouseWorker, RotateCameraWorker>() },
				{ WorkerSetType::PointAtMode, MouseControlWorkerSetDef::Create<AttackWorker, PointAtMouseWorker, LeftRightWorker>() },
			};

			return workerSetDefs;
		}

		const MouseControlWorkerSetDef& GetWorkerSetNoneDef()
		{
			static const MouseControlWorkerSetDef noneSetDef = MouseControlWorkerSetDef::Create<void, void, void>();

			return noneSetDef;
		}
	}

	const MouseControlWorkerSetDef& GetWorkerSetDef(WorkerSetType type)
	{
		const auto& workerSetDefs = GetWorkerSetDefs();
		const auto workerSetDefIt = workerSetDefs.find(type);
		return workerSetDefIt != workerSetDefs.end() ? workerSetDefIt->second : GetWorkerSetNoneDef();
	}

	const std::unique_ptr<MouseControlWorker> CreateWorker(WorkerType type)
	{
		const auto& factories = GetWorkerFactories();
		const auto factoryIt = factories.find(type);
		return factoryIt != factories.end() ? factoryIt->second() : nullptr;
	}
}

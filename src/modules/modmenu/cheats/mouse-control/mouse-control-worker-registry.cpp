#include "mouse-control-worker.h"
#include "attack-worker.h"
#include "locked-mouse-worker.h"
#include "point-at-mouse-worker.h"
#include "left-right-worker.h"
#include "mouse-control-worker-registry.h"

namespace ModMenuModule::MouseControlWorkerRegistry {
	namespace {
		using WorkerFactory = std::unique_ptr<MouseControlWorker>(*)();

		template<typename WorkerT>
		std::unique_ptr<MouseControlWorker> CreateWorkerInstance()
		{
			return std::make_unique<WorkerT>();
		}

		const std::unordered_map<WorkerType, WorkerFactory>& GetWorkerFactories()
		{
			static const std::unordered_map<WorkerType, WorkerFactory> factories = {
				{ GetWorkerType<AttackWorker>(), &CreateWorkerInstance<AttackWorker> },
				{ GetWorkerType<LockedMouseWorker>(), &CreateWorkerInstance<LockedMouseWorker> },
				{ GetWorkerType<PointAtMouseWorker>(), &CreateWorkerInstance<PointAtMouseWorker> },
				{ GetWorkerType<LeftRightWorker>(), &CreateWorkerInstance<LeftRightWorker> },
			};

			return factories;
		}

		const std::unordered_map<WorkerSetType, MouseControlWorkerSetDef>& GetWorkerSetDefs()
		{
			static const std::unordered_map<WorkerSetType, MouseControlWorkerSetDef> workerSetDefs = {
				{ WorkerSetType::RotateMode, MouseControlWorkerSetDef::Create<AttackWorker, LockedMouseWorker, LeftRightWorker>() },
				{ WorkerSetType::RotateModeInCar, MouseControlWorkerSetDef::Create<AttackWorker, void, void>() },
				{ WorkerSetType::PointAtMode, MouseControlWorkerSetDef::Create<AttackWorker, PointAtMouseWorker, LeftRightWorker>() },
				{ WorkerSetType::PointAtModeInCar, MouseControlWorkerSetDef::Create<AttackWorker, void, void>() },
			};

			return workerSetDefs;
		}
	}

	const MouseControlWorkerSetDef& GetWorkerSetDef(WorkerSetType type)
	{
		return GetWorkerSetDefs().at(type);
	}

	const std::unique_ptr<MouseControlWorker> CreateWorker(WorkerType type)
	{
		const auto& factories = GetWorkerFactories();
		const auto factoryIt = factories.find(type);
		return factoryIt != factories.end() ? factoryIt->second() : nullptr;
	}
}

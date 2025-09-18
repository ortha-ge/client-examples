module;

#include <utility>

#include <entt/entt.hpp>

module Game.CameraControllerSystem;

import Core.Spatial;
import Game.CameraController;

namespace Game {

	CameraControllerSystem::CameraControllerSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler, Core::Timer& timer)
		: mRegistry(registry)
		, mScheduler(scheduler)
		, mTimer(timer) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	CameraControllerSystem::~CameraControllerSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void CameraControllerSystem::tickSystem(entt::registry& registry) {
		using namespace Core;

		registry.view<CameraController, Spatial>()
			.each([this](CameraController& controller, Spatial& spatial) {
				if (controller.tickTock) {
					spatial.position.x -= mTimer.getDeltaT() * 25.0f;
					if (spatial.position.x < -50.0f) {
						controller.tickTock = false;
					}
				} else {
					spatial.position.x += mTimer.getDeltaT() * 25.0f;
					if (spatial.position.x > 50.0f) {
						controller.tickTock = true;
					}
				}
			});
	}

} // namespace Game

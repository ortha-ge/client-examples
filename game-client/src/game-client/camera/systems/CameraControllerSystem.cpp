module;

#include <cmath>
#include <utility>

module Game.CameraControllerSystem;

import Core.GlobalSpatial;
import Core.Spatial;
import Game.CameraController;
import Gfx.Camera;
import Gfx.Viewport;
import entt;
import glm;

namespace Game {

	CameraControllerSystem::CameraControllerSystem(entt::registry& registry, Core::Scheduler& scheduler, Core::Timer& timer)
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
			.each([this, &registry](CameraController& controller, Spatial& spatial) {
				if (!controller.followEntity) {
					return;
				}

				if (!registry.all_of<GlobalSpatial>(*controller.followEntity)) {
					return;
				}



				const auto& followSpatial{ registry.get<GlobalSpatial>(*controller.followEntity) };
				glm::vec3 lookAt = followSpatial.position + controller.followOffset - glm::vec3(680, 384, 0.0f);

				const float distance = glm::distance(spatial.position, lookAt);
				const float lerpFactor = (distance / 10.0f) * mTimer.getDeltaT();

				spatial.position.x = std::lerp(spatial.position.x, lookAt.x, lerpFactor);
				spatial.position.y = std::lerp(spatial.position.y, lookAt.y, lerpFactor);;
			});
	}

} // namespace Game

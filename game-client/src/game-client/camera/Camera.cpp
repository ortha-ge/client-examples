module;

#include <optional>

module Game.Camera;

import Ortha.Core.EnTTNode;
import Ortha.Core.Spatial;
import Game.CameraController;
import Ortha.Gfx.Camera;
import Ortha.Gfx.Viewport;
import glm;

namespace Game {

	entt::entity createCamera(entt::registry& registry, Core::Scheduler& scheduler, Core::Timer& timer) {
		using namespace Core;
		using namespace Ortha::Gfx;

		const entt::entity cameraEntity = createEnTTNode(registry, "Camera");
		registry.emplace<CameraController>(cameraEntity, std::nullopt, glm::vec3{ 0.0f, -100.0f, 0.0f });
		registry.emplace<Spatial>(cameraEntity);
		registry.emplace<Viewport>(cameraEntity);
		registry.emplace<Camera>(cameraEntity, Camera {
			.viewport = cameraEntity
		});
		return cameraEntity;
	}

} // namespace Game

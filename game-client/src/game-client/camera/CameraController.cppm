module;

#include <entt/entt.hpp>

#include <glm/vec3.hpp>

export module Game.CameraController;

import Core.Scheduler;
import Core.Timer;

export namespace Game {

	struct CameraController {
		std::optional<entt::entity> followEntity;
		glm::vec3 followOffset;
	};

} // namespace Game

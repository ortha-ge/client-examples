module;

#include <optional>

export module Game.CameraController;

import Ortha.Core.Scheduler;
import Ortha.Core.Timer;
import entt;
import glm;

export namespace Game {

	struct CameraController {
		std::optional<entt::entity> followEntity;
		glm::vec3 followOffset;
	};

} // namespace Game

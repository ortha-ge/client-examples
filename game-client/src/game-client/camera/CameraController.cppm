module;

#include <entt/entt.hpp>

export module Game.CameraController;

import Core.Scheduler;
import Core.Timer;

export namespace Game {

	struct CameraController {
		bool tickTock{ false };
	};

} // namespace Game

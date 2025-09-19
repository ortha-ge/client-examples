module;

#include <entt/fwd.hpp>

export module Game.MainMenuControllerSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Game {

	class MainMenuControllerSystem {
	public:

		MainMenuControllerSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~MainMenuControllerSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Game

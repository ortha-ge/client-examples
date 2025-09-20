module;

#include <entt/fwd.hpp>

export module Game.HUDControllerSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Game {

	class HUDControllerSystem {
	public:

		HUDControllerSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~HUDControllerSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

}
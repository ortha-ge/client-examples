module;

#include <entt/fwd.hpp>

export module Game.CharacterControllerSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Game {

	class CharacterControllerSystem {
	public:

		CharacterControllerSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~CharacterControllerSystem();

		static void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Game

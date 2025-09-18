module;

#include <entt/fwd.hpp>

export module Game.CameraControllerSystem;

import Core.EnTTRegistry;
import Core.Scheduler;
import Core.Timer;

export namespace Game {

	class CameraControllerSystem {
	public:

		CameraControllerSystem(Core::EnTTRegistry&, Core::Scheduler&, Core::Timer&);
		~CameraControllerSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::Timer& mTimer;
		Core::TaskHandle mTickHandle;

	};

} // namespace Game

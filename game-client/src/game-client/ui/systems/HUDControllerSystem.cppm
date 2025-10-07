export module Game.HUDControllerSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Game {

	class HUDControllerSystem {
	public:

		HUDControllerSystem(entt::registry&, Ortha::Core::Scheduler&);
		~HUDControllerSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Ortha::Core::Scheduler& mScheduler;
		Ortha::Core::TaskHandle mTickHandle;

	};

}
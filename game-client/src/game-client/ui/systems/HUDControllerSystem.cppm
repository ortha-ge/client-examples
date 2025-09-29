export module Game.HUDControllerSystem;

import Core.Scheduler;
import entt;

export namespace Game {

	class HUDControllerSystem {
	public:

		HUDControllerSystem(entt::registry&, Core::Scheduler&);
		~HUDControllerSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

}
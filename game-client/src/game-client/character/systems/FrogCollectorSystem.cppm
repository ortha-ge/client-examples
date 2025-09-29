export module Game.FrogCollectorSystem;

import Core.Scheduler;
import entt;

export namespace Game {

	class FrogCollectorSystem {
	public:

		FrogCollectorSystem(entt::registry&, Core::Scheduler&);
		~FrogCollectorSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Game

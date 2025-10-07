export module Game.FrogCollectorSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Game {

	class FrogCollectorSystem {
	public:

		FrogCollectorSystem(entt::registry&, Ortha::Core::Scheduler&);
		~FrogCollectorSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Ortha::Core::Scheduler& mScheduler;
		Ortha::Core::TaskHandle mTickHandle;

	};

} // namespace Game

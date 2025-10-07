export module Game.CalculatePlayerScoreSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Game {

	class CalculatePlayerScoreSystem {
	public:

		CalculatePlayerScoreSystem(entt::registry&, Ortha::Core::Scheduler&);
		~CalculatePlayerScoreSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Ortha::Core::Scheduler& mScheduler;
		Ortha::Core::TaskHandle mTickHandle;

	};

} // namespace Game

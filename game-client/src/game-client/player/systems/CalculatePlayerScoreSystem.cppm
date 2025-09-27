module;

#include <entt/fwd.hpp>

export module Game.CalculatePlayerScoreSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Game {

	class CalculatePlayerScoreSystem {
	public:

		CalculatePlayerScoreSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~CalculatePlayerScoreSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Game

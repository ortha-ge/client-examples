export module Game.MainMenuControllerSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Game {

	class MainMenuControllerSystem {
	public:

		MainMenuControllerSystem(entt::registry&, Ortha::Core::Scheduler&);
		~MainMenuControllerSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Ortha::Core::Scheduler& mScheduler;
		Ortha::Core::TaskHandle mTickHandle;

	};

} // namespace Game

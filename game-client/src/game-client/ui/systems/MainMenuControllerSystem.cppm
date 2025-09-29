export module Game.MainMenuControllerSystem;

import Core.Scheduler;
import entt;

export namespace Game {

	class MainMenuControllerSystem {
	public:

		MainMenuControllerSystem(entt::registry&, Core::Scheduler&);
		~MainMenuControllerSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Game

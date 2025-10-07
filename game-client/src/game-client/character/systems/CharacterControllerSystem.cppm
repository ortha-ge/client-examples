export module Game.CharacterControllerSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Game {

	class CharacterControllerSystem {
	public:

		CharacterControllerSystem(entt::registry&, Core::Scheduler&);
		~CharacterControllerSystem();

		static void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace Game

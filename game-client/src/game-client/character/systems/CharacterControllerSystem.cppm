export module Game.CharacterControllerSystem;

import Ortha.Core.Scheduler;
import entt;

export namespace Game {

	class CharacterControllerSystem {
	public:

		CharacterControllerSystem(entt::registry&, Ortha::Core::Scheduler&);
		~CharacterControllerSystem();

		static void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Ortha::Core::Scheduler& mScheduler;
		Ortha::Core::TaskHandle mTickHandle;

	};

} // namespace Game

module;

#include <random>

export module Game.CharacterSpawnerSystem;

import Core.Scheduler;
import Core.Spatial;
import Core.Timer;
import Game.CharacterSpawner;
import entt;

export namespace Game {

	class CharacterSpawnerSystem {
	public:

		CharacterSpawnerSystem(entt::registry&, Core::Scheduler&, Core::Timer&);
		~CharacterSpawnerSystem();

		void tickSystem(entt::registry&);
		void trySpawnCharacter(entt::registry&, const CharacterSpawner&, const Core::Spatial&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::Timer& mTimer;
		Core::TaskHandle mTickHandle;

		std::mt19937 mRandom;

	};

} // namespace Game

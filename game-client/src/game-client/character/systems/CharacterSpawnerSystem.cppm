module;

#include <random>

export module Game.CharacterSpawnerSystem;

import Ortha.Core.Scheduler;
import Ortha.Core.Spatial;
import Ortha.Core.Timer;
import Game.CharacterSpawner;
import entt;

export namespace Game {

	class CharacterSpawnerSystem {
	public:

		CharacterSpawnerSystem(entt::registry&, Ortha::Core::Scheduler&, Ortha::Core::Timer&);
		~CharacterSpawnerSystem();

		void tickSystem(entt::registry&);
		void trySpawnCharacter(entt::registry&, const CharacterSpawner&, const Ortha::Core::Spatial&);

	private:

		entt::registry& mRegistry;
		Ortha::Core::Scheduler& mScheduler;
		Ortha::Core::Timer& mTimer;
		Ortha::Core::TaskHandle mTickHandle;

		std::mt19937 mRandom;

	};

} // namespace Game

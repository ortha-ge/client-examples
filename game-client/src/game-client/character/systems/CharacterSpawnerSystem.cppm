module;

#include <random>

#include <entt/fwd.hpp>

export module Game.CharacterSpawnerSystem;

import Core.EnTTRegistry;
import Core.Scheduler;
import Core.Spatial;
import Core.Timer;
import Game.CharacterSpawner;

export namespace Game {

	class CharacterSpawnerSystem {
	public:

		CharacterSpawnerSystem(Core::EnTTRegistry&, Core::Scheduler&, Core::Timer&);
		~CharacterSpawnerSystem();

		void tickSystem(entt::registry&);
		void trySpawnCharacter(entt::registry&, const CharacterSpawner&, const Core::Spatial&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::Timer& mTimer;
		Core::TaskHandle mTickHandle;

		std::mt19937 mRandom;

	};

} // namespace Game

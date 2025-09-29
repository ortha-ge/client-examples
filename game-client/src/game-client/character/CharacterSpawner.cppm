module;

#include <chrono>

export module Game.CharacterSpawner;

import entt;

export namespace Game {

	struct CharacterSpawner {
		entt::entity sceneRoot{ entt::null };
		std::chrono::steady_clock::time_point mNextSpawn{};
		uint32_t mSpawnDelayResetMinMS{ 1000 };
		uint32_t mSpawnDelayResetMaxMS{ 5000 };
	};

} // namespace Game

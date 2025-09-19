module;

#include <chrono>

#include <entt/entt.hpp>

export module Game.CharacterSpawner;

export namespace Game {

	struct CharacterSpawner {
		entt::entity sceneRoot{ entt::null };
		std::chrono::steady_clock::time_point mNextSpawn{};
	};

} // namespace Game

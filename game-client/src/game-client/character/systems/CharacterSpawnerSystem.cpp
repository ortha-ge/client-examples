module;

#include <chrono>
#include <random>
#include <utility>

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

module Game.CharacterSpawnerSystem;

import Core.EnTTNode;
import Core.Spatial;
import Game.Character;
import Game.CharacterController;
import Game.CharacterSpawner;
import Input.KeyboardState;
import Input.MouseState;

namespace Game::CharacterSpawnerSystemInternal {

	entt::entity createFrog(entt::registry& registry, Core::Scheduler& scheduler, Core::Timer& timer, glm::vec2 pos) {
		static const CharacterResourceConfig frogResourceConfig {
			"assets/materials/frog.json",
			"assets/sprites/frog.json",
			"assets/collision_shapes/frog.json",
			"assets/sounds/frog_ribbit.ogg"
		};
		return createCharacter(registry, frogResourceConfig, scheduler, timer,
			{ pos.x, pos.y, 1.0f }, { 2.0f, 2.0f, 1.0f }, 1.0f);
	}

	void resetCharacterSpawner(CharacterSpawner& characterSpawner, std::mt19937& random) {
		std::uniform_int_distribution<int64_t> distribution(1000, 5000);
		const int64_t randomMilliseconds = distribution(random);

		characterSpawner.mNextSpawn = std::chrono::steady_clock::now() + std::chrono::milliseconds(randomMilliseconds);
	}

} // namespace Game::CharacterSpawnerSystemInternal

namespace Game {

	CharacterSpawnerSystem::CharacterSpawnerSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler, Core::Timer& timer)
		: mRegistry(registry)
		, mScheduler(scheduler)
		, mTimer(timer) {

		mRandom = std::mt19937{ std::random_device{}() };
		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	CharacterSpawnerSystem::~CharacterSpawnerSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void CharacterSpawnerSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		using namespace CharacterSpawnerSystemInternal;

		registry.view<CharacterSpawner, Spatial>()
			.each([this, &registry](CharacterSpawner& characterSpawner, const Spatial& spatial) {
				if (characterSpawner.mNextSpawn == std::chrono::steady_clock::time_point{}) {
					resetCharacterSpawner(characterSpawner, mRandom);
					return;
				}

				const bool canSpawn = std::chrono::steady_clock::now() >= characterSpawner.mNextSpawn;
				if (canSpawn) {
					resetCharacterSpawner(characterSpawner, mRandom);
					trySpawnCharacter(registry, characterSpawner, spatial);
				}
			});
	}

	void CharacterSpawnerSystem::trySpawnCharacter(
		entt::registry& registry, const CharacterSpawner& spawner, const Core::Spatial& spatial) {
		using namespace Core;
		using namespace CharacterSpawnerSystemInternal;

		const glm::vec2 spawnPosition{ spatial.position.x, spatial.position.y };
		const auto renderObjectEntity = createFrog(registry, mScheduler, mTimer, spawnPosition);
		addChildNode(registry, spawner.sceneRoot, renderObjectEntity);
	}

} // namespace Game

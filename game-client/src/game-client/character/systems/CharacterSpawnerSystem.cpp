module;

#include <chrono>
#include <random>
#include <utility>

module Game.CharacterSpawnerSystem;

import Ortha.Core.EnTTNode;
import Ortha.Core.Spatial;
import Game.CharacterSpawner;
import Game.FrogCharacter;
import Ortha.Input.KeyboardState;
import Ortha.Input.MouseState;
import entt;
import glm;

namespace Game::CharacterSpawnerSystemInternal {

	void resetCharacterSpawner(CharacterSpawner& characterSpawner, std::mt19937& random) {
		std::uniform_int_distribution<int64_t> distribution(characterSpawner.mSpawnDelayResetMinMS, characterSpawner.mSpawnDelayResetMaxMS);
		const int64_t randomMilliseconds = distribution(random);

		characterSpawner.mSpawnDelayResetMinMS = std::max(800u, characterSpawner.mSpawnDelayResetMinMS - 100u);
		characterSpawner.mSpawnDelayResetMaxMS = std::max(1000u, characterSpawner.mSpawnDelayResetMaxMS - 100u);

		characterSpawner.mNextSpawn = std::chrono::steady_clock::now() + std::chrono::milliseconds(randomMilliseconds);
	}

} // namespace Game::CharacterSpawnerSystemInternal

namespace Game {

	CharacterSpawnerSystem::CharacterSpawnerSystem(entt::registry& registry, Core::Scheduler& scheduler, Core::Timer& timer)
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
		const auto renderObjectEntity = createFrogCharacter(registry, spawnPosition);
		addChildNode(registry, spawner.sceneRoot, renderObjectEntity);
	}

} // namespace Game

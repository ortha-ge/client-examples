module;

#include <string>
#include <utility>

module Game.FrogCollectorSystem;

import Audio.PlaySoundSourceRequest;
import Core.Node;
import Core.Spatial;
import Game.Character;
import Game.FrogCollector;
import Physics2d.CollisionEvent;

namespace Game {

	FrogCollectorSystem::FrogCollectorSystem(entt::registry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	FrogCollectorSystem::~FrogCollectorSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void FrogCollectorSystem::tickSystem(entt::registry& registry) {
		using namespace Audio;
		using namespace Core;
		using namespace Physics2d;

		registry.view<CollisionEvent>()
			.each([&registry](const CollisionEvent& collisionEvent) {
				entt::entity collectorEntity = entt::null;
				entt::entity frogEntity = entt::null;
				if (registry.all_of<FrogCollector>(collisionEvent.primary)) {
					collectorEntity = collisionEvent.primary;
					frogEntity = collisionEvent.secondary;
				} else if (registry.all_of<FrogCollector>(collisionEvent.secondary)) {
					collectorEntity = collisionEvent.secondary;
					frogEntity = collisionEvent.primary;
				} else {
					return;
				}

				auto& frogCollector{ registry.get<FrogCollector>(collectorEntity) };
				if (!registry.all_of<Character, NodeHandle>(frogEntity)) {
					return;
				}

				auto&& [character, frogNode] = registry.get<Character, NodeHandle>(frogEntity);
				if (character.tag == "frog") {
					frogNode->destroy();
					frogCollector.collectedCount += 1;

					auto& collectorSpatial{ registry.get<Spatial>(collectorEntity) };
					collectorSpatial.scale.x += 0.05f;
					collectorSpatial.scale.y += 0.05f;

					registry.get_or_emplace<PlaySoundSourceRequest>(collectorEntity) = PlaySoundSourceRequest {
						.volumeScale = 4.0f
					};
				}
			});
	}

} // namespace Game

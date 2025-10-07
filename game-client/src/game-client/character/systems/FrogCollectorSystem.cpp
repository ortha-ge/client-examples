module;

#include <string>
#include <utility>

module Game.FrogCollectorSystem;

import Ortha.Audio.PlaySoundSourceRequest;
import Ortha.Core.Node;
import Ortha.Core.NodeHandle;
import Ortha.Core.Spatial;
import Game.Character;
import Game.FrogCollector;
import Ortha.Physics2d.CollisionEvent;

namespace Game {

	FrogCollectorSystem::FrogCollectorSystem(entt::registry& registry, Ortha::Core::Scheduler& scheduler)
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
		using namespace Ortha::Audio;
		using namespace Ortha::Core;
		using namespace Ortha::Physics2d;

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

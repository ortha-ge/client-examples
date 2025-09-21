module;

#include <utility>

#include <entt/entt.hpp>

module Game.HUDControllerSystem;

import Core.EnTTNode;
import Core.Node;
import Game.HUD;
import Gfx.FontObject;

namespace Game {

	HUDControllerSystem::HUDControllerSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	HUDControllerSystem::~HUDControllerSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void HUDControllerSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		using namespace Gfx;

		registry.view<HUD, NodeHandle>()
			.each([&registry](const NodeHandle& nodeHandle) {
				for (auto&& childNode : nodeHandle.getNode()->getChildren()) {
					if (childNode->getName() != "ScoreText") {
						continue;
					}

					auto scoreTextEnttNode = std::static_pointer_cast<EnTTNode>(childNode);
					const entt::entity scoreTextEntity = scoreTextEnttNode->getEntity();
					if (!registry.all_of<FontObject>(scoreTextEntity)) {
						return;
					}

					static int score = 0;
					auto& fontObject{ registry.get<FontObject>(scoreTextEntity) };
					//fontObject.text = std::format("Score {}", score);
					score += 1000;
				}
			});
	}

} // namespace Game

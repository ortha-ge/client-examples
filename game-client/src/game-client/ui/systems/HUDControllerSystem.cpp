module;

#include <format>
#include <memory>
#include <string>
#include <utility>
#include <vector>

module Game.HUDControllerSystem;

import Core.EnTTNode;
import Core.Node;
import Core.NodeHandle;
import Game.HUD;
import Game.PlayerScore;
import Gfx.FontObject;

namespace Game {

	HUDControllerSystem::HUDControllerSystem(entt::registry& registry, Core::Scheduler& scheduler)
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
			.each([&registry](const HUD& hud, const NodeHandle& nodeHandle) {
				if (!registry.all_of<PlayerScore>(hud.player)) {
					return;
				}

				const auto& playerScore{ registry.get<PlayerScore>(hud.player) };

				for (auto&& childNode : nodeHandle.getNode()->getChildren()) {
					if (childNode->getName() != "ScoreText") {
						continue;
					}

					auto scoreTextEnttNode = std::static_pointer_cast<EnTTNode>(childNode);
					const entt::entity scoreTextEntity = scoreTextEnttNode->getEntity();
					if (!registry.all_of<FontObject>(scoreTextEntity)) {
						return;
					}

					auto& fontObject{ registry.get<FontObject>(scoreTextEntity) };
					fontObject.text = std::format("Score {}", playerScore.score);
				}
			});
	}

} // namespace Game

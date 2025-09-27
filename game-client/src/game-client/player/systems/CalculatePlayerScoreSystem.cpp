module;

#include <utility>

#include <entt/entt.hpp>

module Game.CalculatePlayerScoreSystem;

import Game.Character;
import Game.FrogCollector;
import Game.Player;
import Game.PlayerScore;

namespace Game {

	CalculatePlayerScoreSystem::CalculatePlayerScoreSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	CalculatePlayerScoreSystem::~CalculatePlayerScoreSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void CalculatePlayerScoreSystem::tickSystem(entt::registry& registry) {
		registry.view<Player, PlayerScore>()
			.each([&registry](const Player& player, PlayerScore& playerScore) {
				if (!registry.all_of<FrogCollector>(player.character)) {
					return;
				}

				const auto& frogCollector{ registry.get<FrogCollector>(player.character) };
				playerScore.score = frogCollector.collectedCount * 50;
			});
	}

} // namespace Game

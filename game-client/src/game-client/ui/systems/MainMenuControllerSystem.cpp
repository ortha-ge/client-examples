module;

#include <chrono>
#include <utility>

#include <entt/entt.hpp>

module Game.MainMenuControllerSystem;

import Core.EnTTNode;
import Core.Log;
import Core.Node;
import Core.TypeId;
import Game.MainMenuController;
import UI.Button;

namespace Game {

	MainMenuControllerSystem::MainMenuControllerSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	MainMenuControllerSystem::~MainMenuControllerSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void MainMenuControllerSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		using namespace UI;

		registry.view<MainMenuController, NodeHandle>()
			.each([&registry](const entt::entity entity, MainMenuController& mainMenuController, const NodeHandle& nodeHandle) {
				auto clockNow = std::chrono::steady_clock::now();
				auto nextValidSelectTime = mainMenuController.lastSelectedTime + std::chrono::milliseconds(750);
				if (clockNow < nextValidSelectTime) {
					return;
				}

				for (auto&& childNodeHandle : nodeHandle.getNode()->getChildren()) {
					if (childNodeHandle->getTypeId() != TypeId::get<EnTTNode>()) {
						continue;
					}

					auto childEnTTNode = std::static_pointer_cast<EnTTNode>(childNodeHandle);
					const entt::entity childEntity = childEnTTNode->getEntity();
					if (!registry.all_of<Button>(childEntity)) {
						continue;
					}

					const auto& button{ registry.get<Button>(childEntity) };
					if (button.state != ButtonState::Pressed) {
						continue;
					}

					if (button.text == "Play") {
						mainMenuController.playCallback();
						mainMenuController.lastSelectedTime = clockNow;
					} else if (button.text == "Exit") {
						mainMenuController.quitCallback();
						mainMenuController.lastSelectedTime = clockNow;
					}
				}
			});
	}

} // namespace Game

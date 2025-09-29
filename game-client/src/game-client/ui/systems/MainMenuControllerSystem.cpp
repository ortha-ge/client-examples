module;

#include <chrono>
#include <utility>

module Game.MainMenuControllerSystem;

import Core.EnTTNode;
import Core.Log;
import Core.Node;
import Core.TypeId;
import Game.MainMenuController;
import UI.Button;
import UI.ImageButton;

namespace Game::MainMenuControllerSystemInternal {

	void processMenuButtonState(MainMenuController& mainMenuController, const UI::Button& button) {
		using namespace UI;
		if (button.state != ButtonState::Pressed) {
			return;
		}

		auto clockNow = std::chrono::steady_clock::now();
		if (button.text == "Play") {
			mainMenuController.playCallback();
			mainMenuController.lastSelectedTime = clockNow;
		} else if (button.text == "Exit") {
			mainMenuController.quitCallback();
			mainMenuController.lastSelectedTime = clockNow;
		}
	}

} // namespace Game::MainMenuControllerSystemInternal

namespace Game {

	MainMenuControllerSystem::MainMenuControllerSystem(entt::registry& registry, Core::Scheduler& scheduler)
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
		using namespace MainMenuControllerSystemInternal;

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
					if (registry.all_of<Button>(childEntity)) {
						const auto& button{ registry.get<Button>(childEntity) };
						processMenuButtonState(mainMenuController, button);
					} else if (registry.all_of<ImageButton>(childEntity)) {
						const auto& button{ registry.get<ImageButton>(childEntity) };
						processMenuButtonState(mainMenuController, button);
					}
				}
			});
	}

} // namespace Game

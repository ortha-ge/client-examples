module;

#include <chrono>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>

module Game.MainMenu;

import Core.EnTTNode;
import Core.Node;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Game.MainMenuController;
import Gfx.ImageDescriptor;
import UI.Button;
import UI.ImageButton;

namespace Game {

	entt::entity createMainMenu(entt::registry& registry, std::function<void()> playCallback, std::function<void()> quitCallback) {
		using namespace Core;
		using namespace UI;

		const entt::entity mainMenuEntity = registry.create();
		registry.emplace<MainMenuController>(mainMenuEntity,
			std::chrono::steady_clock::now(),
			std::move(playCallback),
			std::move(quitCallback)
		);
		registry.emplace<Spatial>(mainMenuEntity, glm::vec3(500.0f, 200.0f, 0.0f));
		registry.emplace<NodeHandle>(mainMenuEntity, NodeHandle::create<EnTTNode>("MainMenu", entt::handle(registry, mainMenuEntity)));

		const entt::entity playButtonEntity{ registry.create() };
		registry.emplace<Spatial>(playButtonEntity, glm::vec3{ 0.0f, 100.0f, 0.0f }, glm::vec3 { 200.0f, 75.0f, 0.0f });
		registry.emplace<Button>(playButtonEntity, "Play",
			glm::vec3{ 0.6f, 0.6f, 0.6f },
			glm::vec3{ 1.0f, 0.0f, 0.0f },
			glm::vec3{ 0.0f, 1.0f, 0.0f});
		registry.emplace<NodeHandle>(playButtonEntity, NodeHandle::create<EnTTNode>("PlayButton", entt::handle(registry, playButtonEntity)));


		const entt::entity exitButtonEntity{ registry.create() };
		registry.emplace<Spatial>(exitButtonEntity, glm::vec3{ 0.0f, 200.0f, 0.0f }, glm::vec3 { 200.0f, 75.0f, 0.0f });
		ImageButton imageButton;
		imageButton.color = glm::vec3{ 0.6f, 0.6f, 0.6f };
		imageButton.hoverColor = glm::vec3{ 0.0f, 0.0f, 1.0f };
		imageButton.pressedColor = glm::vec3{ 0.0f, 1.0f, 0.0f};
		imageButton.text = "Exit";
		imageButton.image = ResourceLoadRequest::create<Gfx::ImageDescriptor>(registry, "assets/textures/button.png");

		registry.emplace<ImageButton>(exitButtonEntity, std::move(imageButton));
		registry.emplace<NodeHandle>(exitButtonEntity, NodeHandle::create<EnTTNode>("ExitButton", entt::handle(registry, exitButtonEntity)));

		addChildNode(registry, mainMenuEntity, playButtonEntity);
		addChildNode(registry, mainMenuEntity, exitButtonEntity);

		return mainMenuEntity;
	}

} // namespace Game

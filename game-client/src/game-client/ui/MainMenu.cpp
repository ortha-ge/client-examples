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
import Gfx.FontDescriptor;
import Gfx.FontObject;
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

		auto fontResource = ResourceLoadRequest::create<Gfx::FontDescriptor>(registry, "assets/fonts/roboto_regular.ttf");
		const entt::entity playTextEntity{ registry.create() };
		registry.emplace<Spatial>(playTextEntity, glm::vec3{ -50.0f, 25.0f, 0.0f });
		registry.emplace<Gfx::FontObject>(playTextEntity, "Play", fontResource);
		registry.emplace<NodeHandle>(playTextEntity, NodeHandle::create<EnTTNode>("PlayText", entt::handle(registry, playTextEntity)));

		const entt::entity exitTextEntity{ registry.create() };
		registry.emplace<Spatial>(exitTextEntity, glm::vec3{ -50.0f, 25.0f, 0.0f });
		registry.emplace<Gfx::FontObject>(exitTextEntity, "Exit", fontResource);
		registry.emplace<NodeHandle>(exitTextEntity, NodeHandle::create<EnTTNode>("ExitText", entt::handle(registry, exitTextEntity)));

		addChildNode(registry, mainMenuEntity, playButtonEntity);
		addChildNode(registry, playButtonEntity, playTextEntity);

		addChildNode(registry, mainMenuEntity, exitButtonEntity);
		addChildNode(registry, exitButtonEntity, exitTextEntity);

		return mainMenuEntity;
	}

} // namespace Game

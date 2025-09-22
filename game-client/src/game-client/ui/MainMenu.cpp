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
		using namespace Gfx;
		using namespace UI;

		const entt::entity mainMenuEntity{ createEnTTNode(registry, "MainMenu") };
		registry.emplace<Spatial>(mainMenuEntity, glm::vec3(500.0f, 200.0f, 0.0f));
		registry.emplace<MainMenuController>(mainMenuEntity,
			std::chrono::steady_clock::now(),
			std::move(playCallback),
			std::move(quitCallback)
		);

		const entt::entity playButtonEntity{ createChildEnTTNode(registry, mainMenuEntity, "PlayButton") };
		registry.emplace<Spatial>(playButtonEntity, glm::vec3{ 0.0f, 100.0f, 0.0f }, glm::vec3 { 200.0f, 75.0f, 0.0f });
		registry.emplace<Button>(playButtonEntity,
			"Play",
			glm::vec3{ 0.6f, 0.6f, 0.6f },
			glm::vec3{ 1.0f, 0.0f, 0.0f },
			glm::vec3{ 0.0f, 1.0f, 0.0f}
		);

		const entt::entity exitButtonEntity{ createChildEnTTNode(registry, mainMenuEntity, "ExitButton") };
		registry.emplace<Spatial>(exitButtonEntity, glm::vec3{ 0.0f, 200.0f, 0.0f }, glm::vec3 { 200.0f, 75.0f, 0.0f });

		ImageButton imageButton;
		imageButton.color = glm::vec3{ 0.6f, 0.6f, 0.6f };
		imageButton.hoverColor = glm::vec3{ 0.0f, 0.0f, 1.0f };
		imageButton.pressedColor = glm::vec3{ 0.0f, 1.0f, 0.0f};
		imageButton.text = "Exit";
		imageButton.image = ResourceLoadRequest::create<ImageDescriptor>(registry, "assets/textures/button.png");
		registry.emplace<ImageButton>(exitButtonEntity, std::move(imageButton));

		auto fontResource = ResourceLoadRequest::create<FontDescriptor>(registry, "assets/fonts/roboto_regular.ttf");

		const entt::entity playTextEntity{ createChildEnTTNode(registry, playButtonEntity, "PlayText") };
		registry.emplace<Spatial>(playTextEntity, glm::vec3{ -50.0f, 25.0f, 0.0f });
		registry.emplace<FontObject>(playTextEntity, "Play", fontResource);

		const entt::entity exitTextEntity{ createChildEnTTNode(registry, exitButtonEntity, "ExitText") };
		registry.emplace<Spatial>(exitTextEntity, glm::vec3{ -50.0f, 25.0f, 0.0f });
		registry.emplace<FontObject>(exitTextEntity, "Exit", fontResource);

		return mainMenuEntity;
	}

} // namespace Game

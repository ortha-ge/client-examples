module;

#include <entt/entt.hpp>
#include <glm/vec3.hpp>

module Game.MainMenu;

import Core.EnTTNode;
import Core.Node;
import Core.Spatial;
import Game.MainMenuController;
import UI.Button;

namespace Game {

	entt::entity createMainMenu(entt::registry& registry) {
		using namespace Core;
		using namespace UI;

		const entt::entity mainMenuEntity = registry.create();
		registry.emplace<MainMenuController>(mainMenuEntity);
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
		registry.emplace<Button>(exitButtonEntity, "Exit",
			glm::vec3{ 0.6f, 0.6f, 0.6f },
			glm::vec3{ 0.0f, 0.0f, 1.0f },
			glm::vec3{ 0.0f, 1.0f, 0.0f});
		registry.emplace<NodeHandle>(exitButtonEntity, NodeHandle::create<EnTTNode>("ExitButton", entt::handle(registry, exitButtonEntity)));

		addChildNode(registry, mainMenuEntity, playButtonEntity);
		addChildNode(registry, mainMenuEntity, exitButtonEntity);

		return mainMenuEntity;
	}

} // namespace Game

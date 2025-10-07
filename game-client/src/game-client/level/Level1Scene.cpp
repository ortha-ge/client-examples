module Game.Level1Scene;

import Ortha.Core.EnTTNode;
import Ortha.Core.Spatial;
import Game.CharacterSpawner;
import Game.Level1Floor;
import glm;

namespace Game {

	entt::entity createLevel1Scene(entt::registry& registry) {
		using namespace Core;

		const entt::entity sceneRootEntity = createEnTTNode(registry, "Scene");

		const entt::entity floorEntity = createLevel1Floor(registry);
		addChildNode(registry, sceneRootEntity, floorEntity);

		createCharacterSpawner(registry, sceneRootEntity, glm::vec2{ 350.0f, 0.0f });
		createCharacterSpawner(registry, sceneRootEntity, glm::vec2{ 650.0f, 0.0f });
		createCharacterSpawner(registry, sceneRootEntity, glm::vec2{ 950.0f, 0.0f });

		return sceneRootEntity;
	}

} // namespace Game

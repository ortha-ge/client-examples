module Game.CharacterSpawner;

import Ortha.Core.EnTTNode;
import Ortha.Core.Spatial;

namespace Game {

	entt::entity createCharacterSpawner(entt::registry& registry, const entt::entity sceneRootEntity, glm::vec2 spawnerPosition) {
		using namespace Core;

		const entt::entity spawnerEntity = createChildEnTTNode(registry, sceneRootEntity, "CharacterSpawner");
		registry.emplace<Spatial>(spawnerEntity, glm::vec3{ spawnerPosition.x, spawnerPosition.y, 1.0f });
		registry.emplace<CharacterSpawner>(spawnerEntity, sceneRootEntity);
		return spawnerEntity;
	}

} // namespace Game

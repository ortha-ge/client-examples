module Game.CatCharacter;

import Game.Character;
import Game.CharacterController;
import Game.FrogCollector;
import entt;
import glm;

namespace Game {

	entt::entity createCatCharacter(entt::registry& registry, glm::vec2 pos) {
		static const CharacterConfig catResourceConfig {
			.name = "Cat",
			.materialFilePath = "assets/materials/cat.json",
			.spriteFilePath = "assets/sprites/cat.json",
			.collisionShapeFilePath = "assets/collision_shapes/cat.json",
			.spawnSoundFilePath = "assets/sounds/cat_meow.wav",
			.tag = "cat",
			.jumpImpulse = 500.0f,
			.moveImpulse = 15.0f
		};
		const entt::entity catEntity =
			createCharacter(registry, catResourceConfig, { pos.x, pos.y, 2.0f }, { 8.0f, 8.0f, 1.0f }, 3.0f);
		registry.emplace<CharacterController>(catEntity);
		registry.emplace<FrogCollector>(catEntity);

		return catEntity;
	}

} // namespace Game

module Game.FrogCharacter;

import Game.Character;

namespace Game {

	entt::entity createFrogCharacter(entt::registry& registry, glm::vec2 pos) {
		static const CharacterConfig frogResourceConfig {
			.name = "Frog",
			.materialFilePath = "assets/materials/frog.json",
			.spriteFilePath = "assets/sprites/frog.json",
			.collisionShapeFilePath =  "assets/collision_shapes/frog.json",
			.spawnSoundFilePath = "assets/sounds/frog_ribbit.ogg",
			.tag = "frog"
		};
		return createCharacter(registry, frogResourceConfig,
			{ pos.x, pos.y, 1.0f }, { 2.0f, 2.0f, 1.0f }, 1.0f);
	}

} // namespace Game

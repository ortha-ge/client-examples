module;

#include <string>

#include <entt/fwd.hpp>
#include <glm/vec3.hpp>

export module Game.Character;

import Core.Scheduler;
import Core.Timer;

export namespace Game {

	struct CharacterConfig {
		std::string materialFilePath;
		std::string spriteFilePath;
		std::string collisionShapeFilePath;
		std::string spawnSoundFilePath;
		float jumpImpulse{ 100000.0f };
		float moveImpulse{ 1000.0f };
	};

	struct Character {
		float jumpImpulse{};
		float moveImpulse{};
	};

	entt::entity createCharacter(entt::registry&, const CharacterConfig&, glm::vec3, glm::vec3, float);

} // namespace Game

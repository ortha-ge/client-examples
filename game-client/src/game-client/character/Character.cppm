module;

#include <string>

export module Game.Character;

import Core.Scheduler;
import Core.Timer;
import entt;
import glm;

export namespace Game {

	struct CharacterConfig {
		std::string name{ "Character" };
		std::string materialFilePath;
		std::string spriteFilePath;
		std::string collisionShapeFilePath;
		std::string spawnSoundFilePath;
		std::string tag;
		float jumpImpulse{ 100000.0f };
		float moveImpulse{ 1000.0f };
	};

	struct Character {
		std::string tag;
		float jumpImpulse{};
		float moveImpulse{};
	};

	entt::entity createCharacter(entt::registry&, const CharacterConfig&, glm::vec3, glm::vec3, float);

} // namespace Game

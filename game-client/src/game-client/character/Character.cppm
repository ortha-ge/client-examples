module;

#include <string>

#include <entt/fwd.hpp>
#include <glm/vec3.hpp>

export module Game.Character;

import Core.Scheduler;
import Core.Timer;

export namespace Game {

	struct CharacterResourceConfig {
		std::string materialFilePath;
		std::string spriteFilePath;
		std::string collisionShapeFilePath;
		std::string spawnSoundFilePath;
	};

	entt::entity createCharacter(entt::registry&, const CharacterResourceConfig&,
		Core::Scheduler&, Core::Timer&, glm::vec3, glm::vec3, float);

} // namespace Game

module;

#include <entt/fwd.hpp>
#include <glm/vec2.hpp>

export module Game.FrogCharacter;

export namespace Game {

	entt::entity createFrogCharacter(entt::registry&, glm::vec2);

} // namespace Game

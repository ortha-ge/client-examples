module;

#include <entt/fwd.hpp>
#include <glm/vec2.hpp>

export module Game.CatCharacter;

export namespace Game {

	entt::entity createCatCharacter(entt::registry&, glm::vec2);

} // namespace CatCharacter

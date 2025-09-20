module;

#include <entt/entt.hpp>

export module Game.HUD;

export namespace Game {

	const entt::entity createHUD(entt::registry&);

	struct HUD {};

} // namespace Game

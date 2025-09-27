module;

#include <entt/entity/entity.hpp>

export module Game.Player;

export namespace Game {

	struct Player {
		entt::entity character{ entt::null };
	};

} // namespace Game

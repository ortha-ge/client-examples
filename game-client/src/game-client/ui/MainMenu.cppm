module;

#include <entt/fwd.hpp>

#include <functional>

export module Game.MainMenu;

export namespace Game {

	entt::entity createMainMenu(entt::registry&, std::function<void()>, std::function<void()>);

} // namespace Game

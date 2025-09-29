module;

#include <functional>

export module Game.MainMenu;

import entt;

export namespace Game {

	entt::entity createMainMenu(entt::registry&, std::function<void()>, std::function<void()>);

} // namespace Game

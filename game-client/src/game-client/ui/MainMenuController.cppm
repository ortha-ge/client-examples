module;

#include <chrono>
#include <functional>

export module Game.MainMenuController;

export namespace Game {

	struct MainMenuController {
		std::chrono::steady_clock::time_point lastSelectedTime;
		std::function<void()> playCallback;
		std::function<void()> quitCallback;
	};

} // namespace Game

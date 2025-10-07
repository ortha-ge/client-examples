
export module Game.MainMenuControllerSystemService;

export import Game.MainMenuControllerSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Game {

	struct MainMenuControllerSystemService
		: kgr::single_service<
			  MainMenuControllerSystem,
			  kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Game

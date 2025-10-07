
export module Game.HUDControllerSystemService;

export import Game.HUDControllerSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Game {

	struct HUDControllerSystemService
		: kgr::single_service<
			  HUDControllerSystem,
			  kgr::dependency<Ortha::Core::EnTTRegistryService, Ortha::Core::SchedulerService>> {};

} // namespace Game

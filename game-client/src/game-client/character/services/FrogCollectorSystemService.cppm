
export module Game.FrogCollectorSystemService;

export import Game.FrogCollectorSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Core.TimerService;

export namespace Game {

	struct FrogCollectorSystemService
		: kgr::single_service<
			  FrogCollectorSystem,
			  kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Game

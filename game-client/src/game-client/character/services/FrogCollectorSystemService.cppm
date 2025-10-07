
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
			  kgr::dependency<Ortha::Core::EnTTRegistryService, Ortha::Core::SchedulerService>> {};

} // namespace Game

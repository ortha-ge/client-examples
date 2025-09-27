
export module Game.FrogCollectorSystemService;

export import Game.FrogCollectorSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Core.TimerService;

export namespace Game {

	struct FrogCollectorSystemService
		: kgr::single_service<
			  FrogCollectorSystem,
			  kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Game

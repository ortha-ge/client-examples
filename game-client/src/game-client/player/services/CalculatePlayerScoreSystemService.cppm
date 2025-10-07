export module Game.CalculatePlayerScoreSystemService;

export import Game.CalculatePlayerScoreSystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Game {

	struct CalculatePlayerScoreSystemService
		: kgr::single_service<
			  CalculatePlayerScoreSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Game

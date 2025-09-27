export module Game.CalculatePlayerScoreSystemService;

export import Game.CalculatePlayerScoreSystem;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;

export namespace Game {

	struct CalculatePlayerScoreSystemService
		: kgr::single_service<
			  CalculatePlayerScoreSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>> {};

} // namespace Game

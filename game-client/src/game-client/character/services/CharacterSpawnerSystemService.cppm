
export module Game.CharacterSpawnerSystemService;

export import Game.CharacterSpawnerSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Core.TimerService;

export namespace Game {

	struct CharacterSpawnerSystemService
		: kgr::single_service<
			  CharacterSpawnerSystem,
			  kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService, Core::TimerService>> {};

} // namespace Game

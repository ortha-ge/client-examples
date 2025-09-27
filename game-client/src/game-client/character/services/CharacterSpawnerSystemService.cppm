
export module Game.CharacterSpawnerSystemService;

export import Game.CharacterSpawnerSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Core.TimerService;

export namespace Game {

	struct CharacterSpawnerSystemService
		: kgr::single_service<
			  CharacterSpawnerSystem,
			  kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService, Core::TimerService>> {};

} // namespace Game

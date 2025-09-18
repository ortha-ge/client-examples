
export module Game.CharacterControllerSystemService;

export import Game.CharacterControllerSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Game {

	struct CharacterControllerSystemService
		: kgr::single_service<CharacterControllerSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>{};

} // namespace Game

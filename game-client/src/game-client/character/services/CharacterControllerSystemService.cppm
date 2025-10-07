
export module Game.CharacterControllerSystemService;

export import Game.CharacterControllerSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Game {

	struct CharacterControllerSystemService
		: kgr::single_service<CharacterControllerSystem, kgr::dependency<Ortha::Core::EnTTRegistryService, Ortha::Core::SchedulerService>>{};

} // namespace Game

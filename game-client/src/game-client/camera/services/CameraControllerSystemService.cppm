
export module Game.CameraControllerSystemService;

export import Game.CameraControllerSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Core.TimerService;

export namespace Game {

	struct CameraControllerSystemService
		: kgr::single_service<
			  CameraControllerSystem,
			  kgr::dependency<Ortha::Core::EnTTRegistryService, Ortha::Core::SchedulerService, Ortha::Core::TimerService>> {};

} // namespace Game

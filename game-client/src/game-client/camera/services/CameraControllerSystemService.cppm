
export module Game.CameraControllerSystemService;

export import Game.CameraControllerSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Core.TimerService;

export namespace Game {

	struct CameraControllerSystemService
		: kgr::single_service<
			  CameraControllerSystem,
			  kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService, Core::TimerService>> {};

} // namespace Game

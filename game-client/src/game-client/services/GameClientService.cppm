
export module Game.ClientService;

export import Game.Client;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Core.TimerService;

export namespace Game {

	void initClientSubSystems(Client&, kgr::container&);

	struct ClientService
		: kgr::single_service<Client, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService, Core::TimerService>>
		, kgr::autocall<kgr::invoke_method<initClientSubSystems>>{};

} // namespace Game

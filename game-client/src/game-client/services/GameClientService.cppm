
export module Game.ClientService;

export import Game.Client;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Core.TimerService;

export namespace Game {

	void initClientSubSystems(Client&, kgr::container&);

	struct ClientService
		: kgr::single_service<Client, kgr::dependency<Ortha::Core::EnTTRegistryService, Ortha::Core::SchedulerService, Ortha::Core::TimerService>>
		, kgr::autocall<kgr::invoke_method<initClientSubSystems>>{};

} // namespace Game

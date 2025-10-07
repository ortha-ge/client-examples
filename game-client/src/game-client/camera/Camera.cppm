export module Game.Camera;

import Ortha.Core.Scheduler;
import Ortha.Core.Timer;
import entt;

export namespace Game {

	entt::entity createCamera(entt::registry&, Ortha::Core::Scheduler&, Ortha::Core::Timer&);

} // namespace Game

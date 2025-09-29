export module Game.Camera;

import Core.Scheduler;
import Core.Timer;
import entt;

export namespace Game {

	entt::entity createCamera(entt::registry&, Core::Scheduler&, Core::Timer&);

} // namespace Game

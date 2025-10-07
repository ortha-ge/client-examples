export module Game.CameraControllerSystem;

import Ortha.Core.Scheduler;
import Ortha.Core.Timer;
import entt;

export namespace Game {

	class CameraControllerSystem {
	public:

		CameraControllerSystem(entt::registry&, Core::Scheduler&, Core::Timer&);
		~CameraControllerSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::Timer& mTimer;
		Core::TaskHandle mTickHandle;

	};

} // namespace Game

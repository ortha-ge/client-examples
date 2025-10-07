export module Game.CameraControllerSystem;

import Ortha.Core.Scheduler;
import Ortha.Core.Timer;
import entt;

export namespace Game {

	class CameraControllerSystem {
	public:

		CameraControllerSystem(entt::registry&, Ortha::Core::Scheduler&, Ortha::Core::Timer&);
		~CameraControllerSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Ortha::Core::Scheduler& mScheduler;
		Ortha::Core::Timer& mTimer;
		Ortha::Core::TaskHandle mTickHandle;

	};

} // namespace Game

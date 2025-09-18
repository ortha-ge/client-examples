module;

#include <entt/entt.hpp>

export module Game.CameraController;

import Core.Scheduler;
import Core.Timer;

export namespace Game {

	class CameraController {
	public:

		CameraController(Core::Scheduler&, Core::Timer&, entt::handle);
		~CameraController();

		void tick();

	private:

		Core::Scheduler& mScheduler;
		Core::Timer& mTimer;
		Core::TaskHandle mTickHandle;

		entt::handle mCameraHandle;

		bool mTickTock{ false };
	};

} // namespace Game

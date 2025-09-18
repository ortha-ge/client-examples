module;

#include <entt/fwd.hpp>
#include <entt/entt.hpp>

export module Game.CharacterController;

import Core.Scheduler;
import Core.Timer;

export namespace Game {

	class CharacterController {
	public:

		CharacterController(Core::Scheduler&, Core::Timer&, entt::handle);
		~CharacterController();

		void tick();

	private:

		Core::Scheduler& mScheduler;
		Core::Timer& mTimer;
		Core::TaskHandle mTickHandle;

		entt::handle mCharacterHandle;

	};

} // namespace Game

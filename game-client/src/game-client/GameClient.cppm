export module Game.Client;

import Ortha.Audio.Systems;
import Ortha.Core.Systems;
import Ortha.Core.Scheduler;
import Ortha.Core.Timer;
import Ortha.DevTools.Systems;
import Ortha.Gfx.Systems;
import Ortha.Input.Systems;
import Ortha.Physics2d.Systems;
//import Ortha.ScriptingJS.Systems;
import Ortha.ScriptingLua.Systems;
import entt;

export namespace Game {

	class Client {
	public:
		Client(entt::registry&, Ortha::Core::Scheduler&, Ortha::Core::Timer&);
		~Client();

		void tickClient(entt::registry&);

	private:

		void createScene(entt::registry&);
		void toggleMainMenu(entt::registry&);

		entt::registry& mRegistry;
		Ortha::Core::Scheduler& mScheduler;
		Ortha::Core::Timer& mTimer;

		entt::entity mPlayerEntity{ entt::null };
		entt::entity mWindowEntity{ entt::null };
		entt::entity mCameraEntity{ entt::null };
		entt::entity mSceneRootEntity{ entt::null };

		Ortha::Core::TaskHandle mTickHandle{};


	};

} // namespace Game

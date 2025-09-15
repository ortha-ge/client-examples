module;

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>

export module Game.Client;

import Audio.Systems;
import Core.Systems;
import Core.EnTTRegistry;
import Core.Scheduler;
import Core.Timer;
import DevTools.Systems;
import Gfx.Systems;
import Input.Systems;
import Physics2d.Systems;
import ScriptingJS.Systems;
import ScriptingLua.Systems;

export namespace Game {

	class Client {
	public:
		Client(
			Core::EnTTRegistry&, Core::Scheduler&, Core::CoreSystems&, Audio::AudioSystems&, Gfx::GfxSystems&,
			Input::InputSystems&, Physics2d::Physics2dSystems&, ScriptingJS::ScriptingJSSystems&,
			ScriptingLua::ScriptingLuaSystems&, DevTools::DevToolsSystems&, Core::Timer&);
		~Client();

	private:

		void createScene(entt::registry& registry);

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::Timer& mTimer;

		entt::entity mWindowEntity{ entt::null };
		entt::entity mCameraEntity{ entt::null };
		entt::entity mSceneRootEntity{ entt::null };

		Core::TaskHandle mTickHandle{};
	};

} // namespace Game

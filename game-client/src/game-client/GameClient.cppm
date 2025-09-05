module;

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>

export module Game.Client;

import Audio.Systems;
import Core.Systems;
import Core.EnTTRegistry;
import Core.Scheduler;
import DevTools.Systems;
import Gfx.Systems;
import Input.Systems;
import ScriptingJS.Systems;
import ScriptingLua.Systems;

export namespace Game {

	class Client {
	public:
		Client(
			Core::EnTTRegistry&, Core::Scheduler&, Core::CoreSystems&, Audio::AudioSystems&, Gfx::GfxSystems&,
			Input::InputSystems&, ScriptingJS::ScriptingJSSystems&, ScriptingLua::ScriptingLuaSystems&,
			DevTools::DevToolsSystems&);
		~Client();

	private:
		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;

		entt::entity mWindowEntity{ entt::null };
		entt::entity mCameraEntity{ entt::null };
		entt::entity mViewportEntity{ entt::null };

		std::vector<entt::entity> mSpawnedRenderObjects;
		entt::entity mBackgroundRenderObject{ entt::null };

		Core::TaskHandle mTickHandle{};
	};

} // namespace Game

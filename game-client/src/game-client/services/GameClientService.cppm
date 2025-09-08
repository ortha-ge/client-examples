
export module Game.ClientService;

import kangaru;
import Audio.SystemsService;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Core.SystemsService;
import Core.TimerService;
import DevTools.SystemsService;
import Gfx.SystemsService;
import Input.SystemsService;
import Physics2d.SystemsService;
import ScriptingJS.SystemsService;
import ScriptingLua.SystemsService;

export import Game.Client;

export namespace Game {

	using ClientServiceDependencies = kgr::dependency<
		Core::EnTTRegistryService, Core::SchedulerService, Core::CoreSystemsService, Audio::AudioSystemsService,
		Gfx::GfxSystemsService, Input::InputSystemsService, Physics2d::Physics2dSystemsService,
		ScriptingJS::ScriptingJSSystemsService, ScriptingLua::ScriptingLuaSystemsService,
		DevTools::DevToolsSystemsService, Core::TimerService>;

	class ClientService : public kgr::single_service<Client, ClientServiceDependencies> {};

} // namespace Game

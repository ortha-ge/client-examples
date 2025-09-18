module;

#include <entt/entt.hpp>

module Game.ClientService;

import Audio.SystemsService;

import Core.SystemsService;
import DevTools.SystemsService;
import Gfx.SystemsService;
import Input.SystemsService;
import Physics2d.SystemsService;
//import ScriptingJS.SystemsService;
//import ScriptingLua.SystemsService;

namespace Game {

	void initClientSubSystems(Client&, kgr::container& container) {
		container.emplace<Core::CoreSystemsService>();
		container.emplace<Gfx::GfxSystemsService>();
		container.emplace<Input::InputSystemsService>();
		container.emplace<Physics2d::Physics2dSystemsService>();
		container.emplace<DevTools::DevToolsSystemsService>();
	}

} // namespace Game

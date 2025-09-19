module;

#include <entt/entt.hpp>

module Game.ClientService;

import Audio.SystemsService;
import Core.SystemsService;
import DevTools.SystemsService;
import Game.CameraControllerSystemService;
import Game.CharacterControllerSystemService;
import Game.CharacterSpawnerSystemService;
import Gfx.SystemsService;
import Input.SystemsService;
import Physics2d.SystemsService;
//import ScriptingJS.SystemsService;
//import ScriptingLua.SystemsService;

namespace Game {

	void initClientSubSystems(Client&, kgr::container& container) {
		container.emplace<Core::CoreSystemsService>();
		container.emplace<Audio::AudioSystemsService>();
		container.emplace<Gfx::GfxSystemsService>();
		container.emplace<Input::InputSystemsService>();
		container.emplace<Physics2d::Physics2dSystemsService>();
		container.emplace<DevTools::DevToolsSystemsService>();

		container.emplace<CameraControllerSystemService>();
		container.emplace<CharacterControllerSystemService>();
		container.emplace<CharacterSpawnerSystemService>();
	}

} // namespace Game

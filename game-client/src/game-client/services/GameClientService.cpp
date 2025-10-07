module Game.ClientService;

import Ortha.Audio.SystemsService;
import Ortha.Core.SystemsService;
import Ortha.DevTools.SystemsService;
import Game.CameraControllerSystemService;
import Game.CharacterControllerSystemService;
import Game.CharacterSpawnerSystemService;
import Game.FrogCollectorSystemService;
import Game.HUDControllerSystemService;
import Game.MainMenuControllerSystemService;
import Game.CalculatePlayerScoreSystemService;
import Ortha.Gfx.SystemsService;
import Ortha.Input.SystemsService;
import Ortha.Physics2d.SystemsService;
import Ortha.UI.SystemsService;
//import Ortha.ScriptingJS.SystemsService;
//import Ortha.ScriptingLua.SystemsService;

namespace Game {

	void initClientSubSystems(Client&, kgr::container& container) {
		using namespace Ortha;
		container.emplace<Core::CoreSystemsService>();
		container.emplace<Audio::AudioSystemsService>();
		container.emplace<Gfx::GfxSystemsService>();
		container.emplace<UI::UISystemsService>();
		container.emplace<Input::InputSystemsService>();
		container.emplace<Physics2d::Physics2dSystemsService>();
		container.emplace<DevTools::DevToolsSystemsService>();

		container.emplace<CameraControllerSystemService>();
		container.emplace<CharacterControllerSystemService>();
		container.emplace<CharacterSpawnerSystemService>();

		container.emplace<FrogCollectorSystemService>();
		container.emplace<CalculatePlayerScoreSystemService>();

		container.emplace<HUDControllerSystemService>();
		container.emplace<MainMenuControllerSystemService>();
	}

} // namespace Game

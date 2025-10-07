module;

#include <chrono>
#include <memory>
#include <random>

#include <tracy/Tracy.hpp>

module Game.Client;

import Ortha.Core.EnTTNode;
import Ortha.Core.NodeHandle;
import Ortha.Core.QuitAppRequest;
import Ortha.Core.Window;
import Game.Camera;
import Game.CameraController;
import Game.CatCharacter;
import Game.LevelBackground;
import Game.Level1Scene;
import Game.Player;
import Game.PlayerScore;
import Game.HUD;
import Game.MainMenu;
import Ortha.Gfx.Camera;
import Ortha.Input.KeyboardEvent;
import Ortha.Input.KeyboardState;
import glm;

namespace Game::GameClientInternal {

	entt::entity createWindow(entt::registry& registry) {
		const entt::entity windowEntity = registry.create();
		registry.emplace<Ortha::Core::Window>(windowEntity, "Ortha - Client Example", 1920, 1080);
		return windowEntity;
	}

	entt::entity createPlayer(entt::registry& registry) {
		const entt::entity playerEntity{ registry.create() };
		registry.emplace<Player>(playerEntity);
		registry.emplace<PlayerScore>(playerEntity);
		return playerEntity;
	}

} // namespace GameClientInternal

namespace Game {

	Client::Client(entt::registry& registry, Ortha::Core::Scheduler& scheduler, Ortha::Core::Timer& timer)
		: mRegistry(registry)
		, mScheduler{ scheduler }
		, mTimer{ timer } {
		using namespace Ortha::Core;
		using namespace GameClientInternal;

		mWindowEntity = createWindow(registry);
		mCameraEntity = createCamera(registry, scheduler, timer);
		const entt::entity backgroundEntity = createBackground(registry);
		addChildNode(registry, mCameraEntity, backgroundEntity);

		mPlayerEntity = createPlayer(registry);

		const entt::entity hudEntity = createHUD(registry, mPlayerEntity);
		addChildNode(registry, mCameraEntity, hudEntity);

		createScene(registry);

		mTickHandle = scheduler.schedule([this] {
			tickClient(mRegistry);
		});
	}

	Client::~Client() {
		mScheduler.unschedule(std::move(mTickHandle));
		mRegistry.destroy(mSceneRootEntity);
		mRegistry.destroy(mCameraEntity);
		mRegistry.destroy(mWindowEntity);
	}

	void Client::tickClient(entt::registry& registry) {
		ZoneScopedN("GameClient::tick");
		using namespace Ortha::Core;
		using namespace Ortha::Input;
		using namespace GameClientInternal;

		registry.view<KeyboardEvent>()
			.each([this](const KeyboardEvent& keyboardEvent) {
				entt::registry& registry{ mRegistry };
				if (keyboardEvent.key == Key::Escape &&
					keyboardEvent.eventType == InputEventType::Pressed) {
					toggleMainMenu(registry);
				}
			});

	}

	void Client::createScene(entt::registry& registry) {
		using namespace Ortha::Core;
		using namespace GameClientInternal;

		mSceneRootEntity = createLevel1Scene(registry);

		const entt::entity catEntity = createCatCharacter(registry, glm::vec2{ 650.0f, 650.0f });
		addChildNode(registry, mSceneRootEntity, catEntity);

		registry.get<Player>(mPlayerEntity).character = catEntity;

		registry.get<Ortha::Gfx::Camera>(mCameraEntity).sceneRoot = mSceneRootEntity;
		registry.get<CameraController>(mCameraEntity).followEntity = catEntity;
	}

	void Client::toggleMainMenu(entt::registry& registry) {
		using namespace Ortha::Core;
		if (!registry.all_of<NodeHandle>(mCameraEntity)) {
			return;
		}

		const auto& cameraNodeHandle{ registry.get<NodeHandle>(mCameraEntity) };
		const auto& childNodes{ cameraNodeHandle.getNode()->getChildren() };
		auto mainMenuIt = std::ranges::find_if(childNodes, [](const auto& childNode) {
			return childNode->getName() == "MainMenu";
		});

		const bool hasMainMenu = mainMenuIt != childNodes.end();
		if (hasMainMenu) {
			cameraNodeHandle.getNode()->removeChild(*mainMenuIt);
			(*mainMenuIt)->destroy();
			return;
		}

		auto playCallback = [this] {
			if (mSceneRootEntity != entt::null) {
				entt::registry& registry{ mRegistry };
				if (!registry.all_of<NodeHandle>(mSceneRootEntity)) {
					return;
				}

				auto& node = registry.get<NodeHandle>(mSceneRootEntity);
				node.getNode()->destroy();
				createScene(mRegistry);

				const auto& cameraNodeHandle{ registry.get<NodeHandle>(mCameraEntity) };
				const auto& childNodes{ cameraNodeHandle.getNode()->getChildren() };
				auto mainMenuIt = std::ranges::find_if(childNodes, [](const auto& childNode) {
					return childNode->getName() == "MainMenu";
				});
				cameraNodeHandle.getNode()->removeChild(*mainMenuIt);
				(*mainMenuIt)->destroy();
			}
		};

		auto quitCallback = [this] {
			entt::registry& registry{ mRegistry };
			const entt::entity quitAppEntity = registry.create();
			registry.emplace<QuitAppRequest>(quitAppEntity);
		};
		const entt::entity mainMenuEntity = createMainMenu(mRegistry, std::move(playCallback), std::move(quitCallback));
		addChildNode(mRegistry, mCameraEntity, mainMenuEntity);
	}

} // namespace Game

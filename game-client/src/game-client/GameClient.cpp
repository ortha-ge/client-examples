module;

#include <chrono>
#include <memory>
#include <random>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <tracy/Tracy.hpp>

module Game.Client;

import Audio.AudioSource;
import Audio.SoundDescriptor;
import Audio.PlaySoundSourceRequest;
import Core.EnTTNode;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.JsonTypeLoaderAdapter;
import Core.Log;
import Core.Node;
import Core.QuitAppRequest;
import Core.ResourceHandle;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TemplateNode;
import Core.TypeId;
import Core.TypeLoader;
import Core.Window;
import Game.CameraController;
import Game.CharacterController;
import Game.Character;
import Game.CharacterSpawner;
import Game.HUD;
import Game.MainMenu;
import Gfx.Camera;
import Gfx.MaterialDescriptor;
import Gfx.RenderObject;
import Gfx.SpriteDescriptor;
import Gfx.SpriteObject;
import Gfx.Viewport;
import Gfx.Reflection.MaterialDescriptor;
import Gfx.Reflection.Sprite;
import Input.KeyboardState;
import Input.MouseState;
import Physics2d.ApplyForceRequest;
import Physics2d.CollisionEvent;
import Physics2d.CollisionShape;
import Physics2d.Rigidbody2d;
import Physics2d.Reflection.BoxCollisionShape;
import Physics2d.Reflection.CircleCollisionShape;

namespace Game::GameClientInternal {

	entt::entity createWindow(entt::registry& registry) {
		const entt::entity windowEntity = registry.create();
		registry.emplace<Core::Window>(windowEntity, "Ortha - Client Example", 1360, 768);
		return windowEntity;
	}

	entt::entity createCamera(entt::registry& registry, Core::Scheduler& scheduler, Core::Timer& timer) {
		using namespace Core;
		const entt::entity cameraEntity = registry.create();
		registry.emplace<CameraController>(cameraEntity, std::nullopt, glm::vec3{ 0.0f, -100.0f, 0.0f });

		const entt::handle cameraEntityHandle(registry, cameraEntity);
		auto cameraRootNodeHandle = NodeHandle::create<EnTTNode>(std::format("CameraNode"), cameraEntityHandle);

		registry.emplace<NodeHandle>(cameraEntity, cameraRootNodeHandle);
		registry.emplace<Gfx::Camera>(cameraEntity, 60.0f);
		registry.emplace<Spatial>(cameraEntity);
		registry.emplace<Gfx::Viewport>(cameraEntity, cameraEntity, glm::vec2{ 0.0f, 0.0f}, glm::vec2{ 1.0f, 1.0f });
		return cameraEntity;
	}

	entt::entity createBackground(entt::registry& registry) {
		const auto backgroundMaterial = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, "assets/materials/background.json",
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());

		const auto backgroundSprite = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, "assets/sprites/background.json",
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::SpriteDescriptor>>());

		const auto backgroundSoundResource = Core::ResourceLoadRequest::create<Audio::SoundDescriptor>(
			registry, "assets/sounds/background_crickets.ogg");

		const auto backgroundEntity = registry.create();
		registry.emplace<Core::NodeHandle>(backgroundEntity, Core::NodeHandle::create<Core::EnTTNode>(std::format("BackgroundNode"), entt::handle(registry, backgroundEntity)));
		registry.emplace<Core::Spatial>(backgroundEntity, glm::vec3{ 1000.0f, 750.0f, 0.0f }, glm::vec3{ 2.0f, 2.0f, 1.0f });
		registry.emplace<Gfx::SpriteObject>(backgroundEntity, backgroundSprite);
		registry.emplace<Gfx::RenderObject>(backgroundEntity, backgroundMaterial);
		registry.emplace<Audio::AudioSource>(backgroundEntity, backgroundSoundResource);

		Audio::PlaySoundSourceRequest playSoundRequest;
		playSoundRequest.looping = true;
		registry.emplace<Audio::PlaySoundSourceRequest>(backgroundEntity, playSoundRequest);
		return backgroundEntity;
	}

	entt::entity createSpawner(entt::registry& registry, const entt::entity sceneRootEntity, glm::vec2 spawnerPosition) {
		using namespace Core;

		const entt::entity spawnerEntity = registry.create();
		registry.emplace<Spatial>(spawnerEntity, glm::vec3{ spawnerPosition.x, spawnerPosition.y, 1.0f });
		registry.emplace<CharacterSpawner>(spawnerEntity, sceneRootEntity);
		registry.emplace<NodeHandle>(spawnerEntity, NodeHandle::create<EnTTNode>("Spawner", entt::handle(registry, spawnerEntity)));

		addChildNode(registry, sceneRootEntity, spawnerEntity);
		return spawnerEntity;
	}

	entt::entity createSceneRoot(entt::registry& registry) {
		using namespace Core;

		const entt::entity sceneRootEntity = registry.create();
		registry.emplace<NodeHandle>(sceneRootEntity, NodeHandle::create<EnTTNode>("Scene", entt::handle(registry, sceneRootEntity)));

		createSpawner(registry, sceneRootEntity, glm::vec2{ 350.0f, 0.0f });
		createSpawner(registry, sceneRootEntity, glm::vec2{ 650.0f, 0.0f });
		createSpawner(registry, sceneRootEntity, glm::vec2{ 950.0f, 0.0f });

		return sceneRootEntity;
	}

	entt::entity createFloor(entt::registry& registry) {
		const auto tilesMaterialResourceHandle = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, "assets/materials/tiles.json",
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());

		const auto floorSprite = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, "assets/sprites/floor.json",
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::SpriteDescriptor>>());

		const auto floorEntity = registry.create();
		registry.emplace<Core::NodeHandle>(floorEntity, Core::NodeHandle::create<Core::EnTTNode>(std::format("Floor"), entt::handle(registry, floorEntity)));
		registry.emplace<Core::Spatial>(floorEntity, glm::vec3{ 650.0f, 750.0f, 1.0f }, glm::vec3{ 20.0f, 1.0f, 1.0f });

		registry.emplace<Gfx::RenderObject>(floorEntity, tilesMaterialResourceHandle);
		registry.emplace<Gfx::SpriteObject>(floorEntity, floorSprite);

		Physics2d::BoxCollisionShape boxCollisionDescriptor{
			69.0f,
			70.0f,
		};
		registry.emplace<Physics2d::CollisionShape>(floorEntity, boxCollisionDescriptor);
		registry.emplace<Physics2d::Rigidbody>(floorEntity);
		return floorEntity;
	}

	entt::entity createCat(entt::registry& registry, Core::Scheduler& scheduler, Core::Timer& timer, glm::vec2 pos) {
		static const CharacterConfig catResourceConfig {
			"assets/materials/cat.json",
			"assets/sprites/cat.json",
			"assets/collision_shapes/cat.json",
			"assets/sounds/cat_meow.wav",
			300.0f,
			7.5f
		};
		const entt::entity catEntity = createCharacter(registry, catResourceConfig, scheduler, timer,
			{ pos.x, pos.y, 2.0f }, { 8.0f, 8.0f, 1.0f }, 3.0f);
		registry.emplace<CharacterController>(catEntity);

		return catEntity;
	}

} // namespace GameClientInternal

namespace Game {

	Client::Client(Core::EnTTRegistry& registry, Core::Scheduler& scheduler, Core::Timer& timer)
		: mRegistry(registry)
		, mScheduler{ scheduler }
		, mTimer{ timer } {
		using namespace GameClientInternal;

		mWindowEntity = createWindow(registry);
		mCameraEntity = createCamera(registry, scheduler, timer);
		const entt::entity backgroundEntity = createBackground(registry);
		addChildNode(registry, mCameraEntity, backgroundEntity);

		const entt::entity hudEntity = createHUD(registry);
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
		using namespace Core;
		using namespace GameClientInternal;

		registry.view<Input::KeyboardState>()
			.each([this](const Input::KeyboardState& keyboardState) {
				entt::registry& registry{ mRegistry };
				if (Input::isKeyPressed(keyboardState, Input::Key::Escape)) {
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
							if (!registry.all_of<Core::NodeHandle>(mSceneRootEntity)) {
								return;
							}

							auto& node = registry.get<Core::NodeHandle>(mSceneRootEntity);
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
						registry.emplace<Core::QuitAppRequest>(quitAppEntity);
					};
					const entt::entity mainMenuEntity = createMainMenu(mRegistry, std::move(playCallback), std::move(quitCallback));
					addChildNode(mRegistry, mCameraEntity, mainMenuEntity);
				}
			});

	}

	void Client::createScene(entt::registry& registry) {
		using namespace Core;
		using namespace GameClientInternal;

		mSceneRootEntity = createSceneRoot(registry);
		const entt::entity floorEntity = createFloor(registry);
		addChildNode(registry, mSceneRootEntity, floorEntity);

		const entt::entity catEntity = createCat(registry, mScheduler, mTimer, glm::vec2{ 650.0f, 650.0f });
		addChildNode(registry, mSceneRootEntity, catEntity);

		registry.get<CameraController>(mCameraEntity).followEntity = catEntity;
	}

} // namespace Game

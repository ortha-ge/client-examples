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
import Game.CatCharacter;
import Game.Character;
import Game.CharacterSpawner;
import Game.Player;
import Game.PlayerScore;
import Game.HUD;
import Game.MainMenu;
import Gfx.Camera;
import Gfx.MaterialDescriptor;
import Gfx.RenderObject;
import Gfx.SpriteDescriptor;
import Gfx.SpriteObject;
import Gfx.Tilemap;
import Gfx.TilemapObject;
import Gfx.Viewport;
import Gfx.Reflection.MaterialDescriptor;
import Gfx.Reflection.ShaderProgramDescriptor;
import Gfx.Reflection.Sprite;
import Gfx.Reflection.Tilemap;
import Input.KeyboardEvent;
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
		registry.emplace<Core::Window>(windowEntity, "Ortha - Client Example", 1920, 1080);
		return windowEntity;
	}

	entt::entity createCamera(entt::registry& registry, Core::Scheduler& scheduler, Core::Timer& timer) {
		using namespace Core;
		using namespace Gfx;

		const entt::entity cameraEntity = createEnTTNode(registry, "Camera");
		registry.emplace<CameraController>(cameraEntity, std::nullopt, glm::vec3{ 0.0f, -100.0f, 0.0f });
		registry.emplace<Spatial>(cameraEntity);
		registry.emplace<Viewport>(cameraEntity);
		registry.emplace<Camera>(cameraEntity, Camera {
			.viewport = cameraEntity
		});
		return cameraEntity;
	}

	entt::entity createBackground(entt::registry& registry) {
		using namespace Audio;
		using namespace Core;
		using namespace Gfx;

		const auto backgroundMaterial = ResourceLoadRequest::create<TypeLoader>(
			registry, "assets/materials/background.json",
			std::make_shared<JsonTypeLoaderAdapter<MaterialDescriptor>>());

		const auto backgroundSprite = ResourceLoadRequest::create<TypeLoader>(
			registry, "assets/sprites/background.json",
			std::make_shared<JsonTypeLoaderAdapter<SpriteDescriptor>>());

		const auto backgroundSoundResource = ResourceLoadRequest::create<SoundDescriptor>(
			registry, "assets/sounds/background_crickets.ogg");

		const auto backgroundEntity = createEnTTNode(registry, "BackgroundNode");
		registry.emplace<Spatial>(backgroundEntity, glm::vec3{ 1000.0f, 750.0f, 0.0f }, glm::vec3{ 2.0f, 2.0f, 1.0f });
		registry.emplace<SpriteObject>(backgroundEntity, backgroundSprite);
		registry.emplace<RenderObject>(backgroundEntity, backgroundMaterial);
		registry.emplace<AudioSource>(backgroundEntity, backgroundSoundResource);

		PlaySoundSourceRequest playSoundRequest;
		playSoundRequest.looping = true;
		registry.emplace<PlaySoundSourceRequest>(backgroundEntity, playSoundRequest);
		return backgroundEntity;
	}

	entt::entity createSpawner(entt::registry& registry, const entt::entity sceneRootEntity, glm::vec2 spawnerPosition) {
		using namespace Core;

		const entt::entity spawnerEntity = createChildEnTTNode(registry, sceneRootEntity, "CharacterSpawner");
		registry.emplace<Spatial>(spawnerEntity, glm::vec3{ spawnerPosition.x, spawnerPosition.y, 1.0f });
		registry.emplace<CharacterSpawner>(spawnerEntity, sceneRootEntity);
		return spawnerEntity;
	}

	entt::entity createSceneRoot(entt::registry& registry) {
		using namespace Core;

		const entt::entity sceneRootEntity = createEnTTNode(registry, "Scene");

		createSpawner(registry, sceneRootEntity, glm::vec2{ 350.0f, 0.0f });
		createSpawner(registry, sceneRootEntity, glm::vec2{ 650.0f, 0.0f });
		createSpawner(registry, sceneRootEntity, glm::vec2{ 950.0f, 0.0f });

		return sceneRootEntity;
	}

	entt::entity createFloor(entt::registry& registry) {
		using namespace Core;
		using namespace Gfx;

		auto tilemapTypeLoader = std::make_shared<JsonTypeLoaderAdapter<TilemapDescriptor>>();
		auto levelTileMap = ResourceLoadRequest::create<TypeLoader>(registry, "assets/tilemaps/level1.json", tilemapTypeLoader);

		auto shaderProgramTypeLoader = std::make_shared<JsonTypeLoaderAdapter<ShaderProgramDescriptor>>();
		auto shaderProgram = ResourceLoadRequest::create<TypeLoader>(registry, "assets/shaders/sprite_shader_program.json", shaderProgramTypeLoader);

		// Base Plate
		const auto floorEntity = createEnTTNode(registry, "Floor");
		registry.emplace<Spatial>(floorEntity, glm::vec3{ 650.0f, 750.0f, 10.0f }, glm::vec3{ 20.0f, 1.0f, 1.0f });

		Physics2d::BoxCollisionShape boxCollisionDescriptor{
			100.0f,
			100.0f,
		};
		registry.emplace<Physics2d::CollisionShape>(floorEntity, boxCollisionDescriptor);
		registry.emplace<Physics2d::Rigidbody>(floorEntity);

		// Left end piece
		const auto leftWallColliderEntity = createEnTTNode(registry, "FloorLeftWall");
		registry.emplace<Spatial>(leftWallColliderEntity, glm::vec3{ -300.0f, 650.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
		registry.emplace<Physics2d::CollisionShape>(leftWallColliderEntity, boxCollisionDescriptor);
		registry.emplace<Physics2d::Rigidbody>(leftWallColliderEntity);

		const auto rightWallColliderEntity = createEnTTNode(registry, "FloorRightWall");
		registry.emplace<Spatial>(rightWallColliderEntity, glm::vec3{ 1600.0f, 650.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
		registry.emplace<Physics2d::CollisionShape>(rightWallColliderEntity, boxCollisionDescriptor);
		registry.emplace<Physics2d::Rigidbody>(rightWallColliderEntity);

		const auto floorTilemapObject = createChildEnTTNode(registry, floorEntity, "FloorTilemap");
		registry.emplace<Spatial>(floorTilemapObject, glm::vec3{ -1000.0f, -125.0f, 0.0f });
		registry.emplace<TilemapObject>(floorTilemapObject, levelTileMap, shaderProgram);

		return floorEntity;
	}

	entt::entity createPlayer(entt::registry& registry) {
		const entt::entity playerEntity{ registry.create() };
		registry.emplace<Player>(playerEntity);
		registry.emplace<PlayerScore>(playerEntity);
		return playerEntity;
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
		using namespace Core;
		using namespace GameClientInternal;

		registry.view<Input::KeyboardEvent>()
			.each([this](const Input::KeyboardEvent& keyboardEvent) {
				entt::registry& registry{ mRegistry };
				if (keyboardEvent.key == Input::Key::Escape &&
					keyboardEvent.eventType == Input::InputEventType::Pressed) {
					toggleMainMenu(registry);
				}
			});

	}

	void Client::createScene(entt::registry& registry) {
		using namespace Core;
		using namespace GameClientInternal;

		mSceneRootEntity = createSceneRoot(registry);
		const entt::entity floorEntity = createFloor(registry);
		addChildNode(registry, mSceneRootEntity, floorEntity);

		const entt::entity catEntity = createCatCharacter(registry, glm::vec2{ 650.0f, 650.0f });
		addChildNode(registry, mSceneRootEntity, catEntity);

		registry.get<Player>(mPlayerEntity).character = catEntity;

		registry.get<Gfx::Camera>(mCameraEntity).sceneRoot = mSceneRootEntity;
		registry.get<CameraController>(mCameraEntity).followEntity = catEntity;
	}

	void Client::toggleMainMenu(entt::registry& registry) {
		using namespace Core;
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
			registry.emplace<QuitAppRequest>(quitAppEntity);
		};
		const entt::entity mainMenuEntity = createMainMenu(mRegistry, std::move(playCallback), std::move(quitCallback));
		addChildNode(mRegistry, mCameraEntity, mainMenuEntity);
	}

} // namespace Game

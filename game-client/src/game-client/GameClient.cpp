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
import Core.ResourceHandle;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TypeId;
import Core.TypeLoader;
import Core.Window;
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

namespace GameClientInternal {

	entt::entity createWindow(entt::registry& registry) {
		const entt::entity windowEntity = registry.create();
		registry.emplace<Core::Window>(windowEntity, "Riastradh", 1360, 768);
		return windowEntity;
	}

	entt::entity createCamera(entt::registry& registry) {
		const entt::entity cameraEntity = registry.create();
		registry.emplace<Core::NodeHandle>(cameraEntity, Core::NodeHandle::create<Core::EnTTNode>(std::format("CameraNode"), entt::handle(registry, cameraEntity)));
		registry.emplace<Gfx::Camera>(cameraEntity, 60.0f);
		registry.emplace<Core::Spatial>(cameraEntity);
		registry.emplace<Gfx::Viewport>(cameraEntity, cameraEntity, glm::vec2{ 0.0f, 0.0f}, glm::vec2{ 1.0f, 1.0f });
		return cameraEntity;
	}

	entt::entity createBackground(entt::registry& registry) {
		const auto backgroundMaterial = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, std::string{ "assets/materials/background.json" },
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());

		const auto backgroundSprite = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, std::string{ "assets/sprites/background.json" },
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::SpriteDescriptor>>());

		const auto backgroundSoundResource = Core::ResourceLoadRequest::create<Audio::SoundDescriptor>(
			registry, "./assets/sounds/background_crickets.ogg");

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

	entt::entity createSceneRoot(entt::registry& registry) {
		const entt::entity sceneRootEntity = registry.create();
		registry.emplace<Core::NodeHandle>(sceneRootEntity, Core::NodeHandle::create<Core::EnTTNode>(std::format("Scene"), entt::handle(registry, sceneRootEntity)));
		return sceneRootEntity;
	}

	entt::entity createFloor(entt::registry& registry) {
		const auto tilesMaterialResourceHandle = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, "assets/materials/tiles.json",
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());

		const auto floorSprite = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, std::string{ "assets/sprites/floor.json" },
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::SpriteDescriptor>>());

		const auto floorEntity = registry.create();
		registry.emplace<Core::NodeHandle>(floorEntity, Core::NodeHandle::create<Core::EnTTNode>(std::format("Floor"), entt::handle(registry, floorEntity)));
		registry.emplace<Core::Spatial>(floorEntity, glm::vec3{ 650.0f, 450.0f, 1.0f }, glm::vec3{ 10.0f, 1.0f, 1.0f });

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

	static size_t characterCounter = 0;

	entt::entity createCharacter(
		entt::registry& registry, glm::vec3 pos, glm::vec3 scale, std::string materialPath,
		std::string spritePath, std::string collisionShapePath, std::string soundPath, float soundRequestVolume) {

		++characterCounter;

		const auto materialResource = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, std::move(materialPath),
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());

		const auto spriteResource = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, std::move(spritePath), std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::SpriteDescriptor>>());

		const auto soundResource =
			Core::ResourceLoadRequest::create<Audio::SoundDescriptor>(registry, std::move(soundPath));

		const auto characterRootNodeEntity = registry.create();
		registry.emplace<Core::Spatial>(characterRootNodeEntity, pos, scale);

		const auto collisionShapeResource = Core::ResourceLoadRequest::create<Core::TypeLoader>(registry,
			std::move(collisionShapePath),
			std::make_shared<Core::JsonTypeLoaderAdapter<Physics2d::CollisionShapeDescriptor>>()
		);

		registry.emplace<Physics2d::CollisionShape>(characterRootNodeEntity, collisionShapeResource);
		registry.emplace<Physics2d::Rigidbody>(characterRootNodeEntity, false);

		Core::NodeHandle& characterRootNode{ registry.emplace<Core::NodeHandle>(characterRootNodeEntity, Core::NodeHandle::create<Core::EnTTNode>(std::format("Character_{}", characterCounter), entt::handle(registry, characterRootNodeEntity))) };

		const auto renderObjectEntity = registry.create();
		registry.emplace<Core::Spatial>(renderObjectEntity);

		registry.emplace<Gfx::RenderObject>(renderObjectEntity, materialResource);
		registry.emplace<Gfx::SpriteObject>(renderObjectEntity, spriteResource);

		registry.emplace<Audio::AudioSource>(renderObjectEntity, soundResource);
		registry.emplace<Audio::PlaySoundSourceRequest>(renderObjectEntity, soundRequestVolume);

		auto& childNodeHandle = registry.emplace<Core::NodeHandle>(renderObjectEntity, Core::NodeHandle::create<Core::EnTTNode>("RenderObject", entt::handle(registry, renderObjectEntity)));
		characterRootNode.getNode()->addChild(childNodeHandle.getNode());

		return characterRootNodeEntity;
	}

	entt::entity createCat(entt::registry& registry, glm::vec2 pos) {
		return createCharacter(
			registry, { pos.x, pos.y, 2.0f }, { 8.0f, 8.0f, 1.0f }, "assets/materials/cat.json",
			"assets/sprites/cat.json", "assets/collision_shapes/cat.json", "assets/sounds/cat_meow.wav", 3.0f);
	}

	entt::entity createFrog(entt::registry& registry, glm::vec2 pos) {
		return createCharacter(
			registry, { pos.x, pos.y, 1.0f }, { 2.0f, 2.0f, 1.0f }, "assets/materials/frog.json",
			"assets/sprites/frog.json", "assets/collision_shapes/frog.json", "assets/sounds/frog_ribbit.ogg", 1.0f);
	}

	void addChildNode(entt::registry& registry, const entt::entity parent, const entt::entity child) {
		const auto* parentNodeHandle = registry.try_get<Core::NodeHandle>(parent);
		const auto* childNodeHandle = registry.try_get<Core::NodeHandle>(child);
		if (parentNodeHandle && childNodeHandle) {
			parentNodeHandle->getNode()->addChild(childNodeHandle->getNode());
		}
	}

} // namespace GameClientInternal

namespace Game {

	Client::Client(
		Core::EnTTRegistry& registry, Core::Scheduler& scheduler, Core::CoreSystems&, Audio::AudioSystems&,
		Gfx::GfxSystems&, Input::InputSystems&, Physics2d::Physics2dSystems&, ScriptingJS::ScriptingJSSystems&,
		ScriptingLua::ScriptingLuaSystems&, DevTools::DevToolsSystems&, Core::Timer& timer)
		: mRegistry(registry)
		, mScheduler{ scheduler }
		, mTimer{ timer } {
		using namespace GameClientInternal;

		mWindowEntity = createWindow(registry);
		mCameraEntity = createCamera(registry);
		const entt::entity backgroundEntity = createBackground(registry);
		addChildNode(registry, mCameraEntity, backgroundEntity);

		createScene(registry);

		mTickHandle = scheduler.schedule([this, lastSpawn = std::chrono::steady_clock::time_point{}, tickTock = false] mutable {
			ZoneScoped;
			auto& cameraSpatial = mRegistry.get<Core::Spatial>(mCameraEntity);
			if (tickTock) {
				cameraSpatial.position.x -= mTimer.getDeltaT() * 25.0f;
				if (cameraSpatial.position.x < -50.0f) {
					tickTock = false;
				}
			} else {
				cameraSpatial.position.x += mTimer.getDeltaT() * 25.0f;
				if (cameraSpatial.position.x > 50.0f) {
					tickTock = true;
				}
			}

			if (!mRegistry.all_of<Input::KeyboardState, Input::MouseState>(mWindowEntity)) {
				return;
			}

			const auto& mouseState = mRegistry.get<Input::MouseState>(mWindowEntity);
			const bool canSpawn = (std::chrono::steady_clock::now() - lastSpawn) > std::chrono::milliseconds(150);
			if (canSpawn && Input::isMouseButtonPressed(mouseState, Input::MouseButton::Left)) {
				const bool spawnCat = mouseState.x < (1360.0f * 0.5f);
				lastSpawn = std::chrono::steady_clock::now();

				const auto renderObjectEntity = spawnCat
					? createCat(mRegistry, { mouseState.x, mouseState.y })
					: createFrog(mRegistry, { mouseState.x, mouseState.y });

				addChildNode(mRegistry, mSceneRootEntity, renderObjectEntity);
			}

			const auto& keyboardState = mRegistry.get<Input::KeyboardState>(mWindowEntity);

			float inputX = 0.0f;
			float inputY = 0.0f;
			const float jumpSpeed = 100000.0f;
			const float moveSpeed = 200.0f;

			if (Input::isKeyPressed(keyboardState, Input::Key::Left)) {
				inputX -= moveSpeed;
			} else if (Input::isKeyPressed(keyboardState, Input::Key::Right)) {
				inputX += moveSpeed;
			}

			if (Input::isKeyPressed(keyboardState, Input::Key::Up)) {
				inputY -= jumpSpeed;
			} else if (Input::isKeyPressed(keyboardState, Input::Key::Down)) {
				//inputY += moveSpeed;
			}

			if (inputX != 0.0f || inputY != 0.0f) {
				auto& nodeHandle{ mRegistry.get<Core::NodeHandle>(mSceneRootEntity) };
				for (auto&& childNode : nodeHandle.getNode()->getChildren()) {
					if (!childNode->getName().contains("Character")) {
						continue;
					}

					if (childNode->getTypeId() != Core::TypeId::get<Core::EnTTNode>()) {
						continue;
					}

					auto characterEnttNode = std::static_pointer_cast<Core::EnTTNode>(childNode);
					const entt::entity characterEntity = characterEnttNode->getEntity();

					auto& rigidBody{ mRegistry.get<Physics2d::Rigidbody>(characterEntity) };
					if (rigidBody.isOnGround) {
						auto& applyForceRequest{ mRegistry.get_or_emplace<Physics2d::ApplyForceRequest>(characterEntity) };
						applyForceRequest.force = glm::vec2(inputX, inputY);
						applyForceRequest.type = Physics2d::ForceType::Impulse;
					} else {
						auto& applyForceRequest{ mRegistry.get_or_emplace<Physics2d::ApplyForceRequest>(characterEntity) };
						applyForceRequest.force = glm::vec2(inputX, 0.0f);
						applyForceRequest.type = Physics2d::ForceType::Impulse;
					}
				}
			}

			auto tryDestroyRootNode = [](entt::registry& registry, const entt::entity entity) {
				if (!registry.all_of<Core::NodeHandle>(entity)) {
					return;
				}

				auto& node = registry.get<Core::NodeHandle>(entity);
				node.getNode()->destroy();
			};

			if (Input::isKeyPressed(keyboardState, Input::Key::Space)) {
				if (mSceneRootEntity != entt::null) {
					tryDestroyRootNode(mRegistry, mSceneRootEntity);
					createScene(mRegistry);
				}
			}

			mRegistry.view<Physics2d::CollisionEvent>()
				.each([this](const entt::entity entity, const Physics2d::CollisionEvent& collisionEvent) {
				});
		});
	}

	Client::~Client() {
		mScheduler.unschedule(std::move(mTickHandle));
		mRegistry.destroy(mSceneRootEntity);
		mRegistry.destroy(mCameraEntity);
		mRegistry.destroy(mWindowEntity);
	}

	void Client::createScene(entt::registry& registry) {
		using namespace GameClientInternal;

		mSceneRootEntity = createSceneRoot(registry);
		const entt::entity floorEntity = createFloor(registry);
		addChildNode(registry, mSceneRootEntity, floorEntity);
	}

} // namespace Game

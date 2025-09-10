module;

#include <chrono>
#include <memory>
#include <random>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

module Game.Client;

import Audio.AudioSource;
import Audio.SoundDescriptor;
import Audio.PlaySoundSourceRequest;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.JsonTypeLoaderAdapter;
import Core.Log;
import Core.ResourceHandle;
import Core.ResourceLoadRequest;
import Core.Spatial;
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
import Physics2d.CollisionEvent;
import Physics2d.CollisionShape;
import Physics2d.Rigidbody2d;

namespace GameClientInternal {

	entt::entity createCharacter(
		entt::registry& registry, glm::vec3 pos, glm::vec3 scale, glm::vec2 collisionBox, std::string materialPath,
		std::string spritePath, std::string soundPath, float soundRequestVolume) {

		const auto materialResource = Core::ResourceHandle::create<Core::TypeLoader>(
				std::move(materialPath), std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());

		auto loadRequest = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(loadRequest, materialResource);

		const auto spriteResource = Core::ResourceHandle::create<Core::TypeLoader>(
								std::move(spritePath), std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::SpriteDescriptor>>());
		loadRequest = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(loadRequest, spriteResource);

		const auto soundResource = Core::ResourceHandle::create<Audio::SoundDescriptor>(std::move(soundPath));
		loadRequest = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(loadRequest, soundResource);

		const auto renderObjectEntity = registry.create();
		registry.emplace<Core::Spatial>(renderObjectEntity, pos, scale);

		registry.emplace<Gfx::RenderObject>(renderObjectEntity, materialResource);
		registry.emplace<Gfx::SpriteObject>(renderObjectEntity, spriteResource);

		Physics2d::CircleCollisionShapeDescriptor boxCollisionDescriptor{ collisionBox.x * 0.5f * scale.x };
		registry.emplace<Physics2d::CollisionShape>(renderObjectEntity, boxCollisionDescriptor);
		registry.emplace<Physics2d::Rigidbody>(renderObjectEntity, false);

		registry.emplace<Audio::AudioSource>(renderObjectEntity, soundResource);
		registry.emplace<Audio::PlaySoundSourceRequest>(renderObjectEntity, soundRequestVolume);
		return renderObjectEntity;
	}

	entt::entity createCat(entt::registry& registry, glm::vec2 pos) {
		return createCharacter(
			registry, { pos.x, pos.y, 2.0f }, { 8.0f, 8.0f, 1.0f }, { 10.0f, 10.0f }, "assets/materials/cat.json",
			"assets/sprites/cat.json", "assets/sounds/cat_meow.wav", 3.0f);
	}

	entt::entity createFrog(entt::registry& registry, glm::vec2 pos) {
		return createCharacter(
			registry, { pos.x, pos.y, 1.0f }, { 2.0f, 2.0f, 1.0f }, { 30.0f, 25.0f }, "assets/materials/frog.json",
			"assets/sprites/frog.json", "assets/sounds/frog_ribbit.ogg", 1.0f);
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

		mWindowEntity = mRegistry.create();
		mRegistry.emplace<Core::Window>(mWindowEntity, "Riastradh", 1360, 768);

		mCameraEntity = mRegistry.create();
		mRegistry.emplace<Gfx::Camera>(mCameraEntity, 60.0f);
		mRegistry.emplace<Core::Spatial>(mCameraEntity, glm::vec3{ 0.0f, 0.0f, 0.0f });

		mViewportEntity = mRegistry.create();
		mRegistry.emplace<Gfx::Viewport>(mViewportEntity, mCameraEntity, 0.1f, 0.1f, 0.9f, 0.9f);

		const auto backgroundMaterial = Core::ResourceHandle::create<Core::TypeLoader>(
									std::string{ "assets/materials/background.json" },
									std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());

		auto loadRequest = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(loadRequest, backgroundMaterial);

		const auto backgroundSprite = Core::ResourceHandle::create<Core::TypeLoader>(
								  std::string{ "assets/sprites/background.json" },
								  std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::SpriteDescriptor>>());
		loadRequest = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(loadRequest, backgroundSprite);

		const auto backgroundSoundResource = Core::ResourceHandle::create<Audio::SoundDescriptor>("./assets/sounds/background_crickets.ogg");
		loadRequest = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(loadRequest, backgroundSoundResource);

		const auto backgroundRenderObject = mRegistry.create();
		mRegistry.emplace<Core::Spatial>(backgroundRenderObject, glm::vec3{ 1000.0f, 750.0f, 0.0f }, glm::vec3{ 2.0f, 2.0f, 1.0f });
		mRegistry.emplace<Gfx::SpriteObject>(backgroundRenderObject, backgroundSprite);
		mRegistry.emplace<Gfx::RenderObject>(backgroundRenderObject, backgroundMaterial);
		mRegistry.emplace<Audio::AudioSource>(backgroundRenderObject, backgroundSoundResource);

		Audio::PlaySoundSourceRequest playSoundRequest;
		playSoundRequest.looping = true;
		mRegistry.emplace<Audio::PlaySoundSourceRequest>(backgroundRenderObject, playSoundRequest);

		mBackgroundRenderObject = backgroundRenderObject;

		mSpawnedRenderObjects.reserve(100);

		const auto tilesMaterialResourceHandle = Core::ResourceHandle::create<Core::TypeLoader>(
				"assets/materials/tiles.json", std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>()
			);
		loadRequest = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(loadRequest, tilesMaterialResourceHandle);

		const auto floorSprite = Core::ResourceHandle::create<Core::TypeLoader>(
								  std::string{ "assets/sprites/floor.json" },
								  std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::SpriteDescriptor>>());
		loadRequest = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(loadRequest, floorSprite);

		const auto floorEntity = mRegistry.create();
		mRegistry.emplace<Core::Spatial>(floorEntity, glm::vec3{ 650.0f, 450.0f, 1.0f }, glm::vec3{ 10.0f, 1.0f, 1.0f });

		mRegistry.emplace<Gfx::RenderObject>(floorEntity, tilesMaterialResourceHandle);
		mRegistry.emplace<Gfx::SpriteObject>(floorEntity, floorSprite);

		Physics2d::BoxCollisionShapeDescriptor boxCollisionDescriptor{
			69.0f,
			70.0f,
		};
		mRegistry.emplace<Physics2d::CollisionShape>(floorEntity, boxCollisionDescriptor);
		mRegistry.emplace<Physics2d::Rigidbody>(floorEntity);

		mTickHandle = scheduler.schedule([this, lastSpawn = std::chrono::steady_clock::time_point{}, tickTock = false] mutable {

			//Core::logEntry(mRegistry, "DeltaT: {}", mTimer.getDeltaT());

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
					? GameClientInternal::createCat(mRegistry, { mouseState.x, mouseState.y })
					: GameClientInternal::createFrog(mRegistry, { mouseState.x, mouseState.y });

				mSpawnedRenderObjects.push_back(renderObjectEntity);
			}

			const auto& keyboardState = mRegistry.get<Input::KeyboardState>(mWindowEntity);

			float inputX = 0.0f;
			float inputY = 0.0f;
			const float moveSpeed = 200.0f * mTimer.getDeltaT();

			if (Input::isKeyPressed(keyboardState, Input::Key::Left)) {
				inputX -= moveSpeed;
			} else if (Input::isKeyPressed(keyboardState, Input::Key::Right)) {
				inputX += moveSpeed;
			}

			if (Input::isKeyPressed(keyboardState, Input::Key::Up)) {
				inputY -= moveSpeed;
			} else if (Input::isKeyPressed(keyboardState, Input::Key::Down)) {
				inputY += moveSpeed;
			}

			mRegistry.view<Physics2d::CollisionEvent>()
				.each([this](const entt::entity entity, const Physics2d::CollisionEvent& collisionEvent) {

				});
		});
	}

	Client::~Client() {
		mScheduler.unschedule(std::move(mTickHandle));

		for (auto&& spawnedRenderObject : mSpawnedRenderObjects) {
			mRegistry.destroy(spawnedRenderObject);
		}

		mRegistry.destroy(mWindowEntity);
	}

} // namespace Game

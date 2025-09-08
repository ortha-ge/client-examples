module;

#include <chrono>
#include <memory>
#include <random>

#include <entt/entt.hpp>

module Game.Client;

import Audio.AudioSource;
import Audio.SoundDescriptor;
import Audio.PlaySoundSourceRequest;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.JsonTypeLoaderAdapter;
import Core.Log;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TypeLoader;
import Core.Window;
import Gfx.Camera;
import Gfx.MaterialDescriptor;
import Gfx.RenderObject;
import Gfx.Sprite;
import Gfx.Viewport;
import Gfx.Reflection.MaterialDescriptor;
import Input.KeyboardState;
import Input.MouseState;
import Physics2d.CollisionShape;
import Physics2d.Rigidbody2d;

namespace GameClientInternal {

	struct Vec2 {
		float x{};
		float y{};
	};

	struct Vec3 {
		float x{};
		float y{};
		float z{};
	};

	entt::entity createCharacter(
		entt::registry& registry, Vec3 pos, Vec3 scale, Vec2 collisionBox, std::string materialPath, std::string soundPath,
		float soundRequestVolume) {

		const auto materialResource = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(
			materialResource,
			Core::ResourceLoadRequest::create<Core::TypeLoader>(
				std::move(materialPath), std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>()));

		const auto soundResource = registry.create();
		registry.emplace<Core::ResourceLoadRequest>(
			soundResource, Core::ResourceLoadRequest::create<Audio::SoundDescriptor>(std::move(soundPath)));

		const auto renderObjectEntity = registry.create();
		registry.emplace<Core::Spatial>(renderObjectEntity, pos.x, pos.y, pos.z, scale.x, scale.y, scale.z);

		registry.emplace<Gfx::RenderObject>(renderObjectEntity, materialResource);
		registry.emplace<Gfx::Sprite>(renderObjectEntity);

		Physics2d::BoxCollisionShapeDescriptor boxCollisionDescriptor{
			collisionBox.x,
			collisionBox.y,
		};
		registry.emplace<Physics2d::CollisionShape>(renderObjectEntity, boxCollisionDescriptor);
		registry.emplace<Physics2d::Rigidbody>(renderObjectEntity, false);

		registry.emplace<Audio::AudioSource>(renderObjectEntity, soundResource);
		registry.emplace<Audio::PlaySoundSourceRequest>(renderObjectEntity, false, soundRequestVolume);
		return renderObjectEntity;
	}

	entt::entity createCat(entt::registry& registry, Vec2 pos) {
		return createCharacter(
			registry, { pos.x, pos.y, 2.0f }, { 8.0f, 8.0f, 1.0f }, { 10.0f, 10.0f }, "assets/materials/cat.json",
			"assets/sounds/cat_meow.wav", 3.0f);
	}

	entt::entity createFrog(entt::registry& registry, Vec2 pos) {
		return createCharacter(
			registry, { pos.x, pos.y, 1.0f }, { 2.0f, 2.0f, 1.0f }, { 30.0f, 25.0f }, "assets/materials/frog.json",
			"assets/sounds/frog_ribbit.ogg", 1.0f);
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
		mRegistry.emplace<Gfx::Camera>(mCameraEntity, 60.0);
		mRegistry.emplace<Core::Spatial>(mCameraEntity, 0.0f, 0.0f, 0.0f);

		mViewportEntity = mRegistry.create();
		mRegistry.emplace<Gfx::Viewport>(mViewportEntity, mCameraEntity, 0.1f, 0.1f, 0.9f, 0.9f);

		const auto backgroundMaterial = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(
			backgroundMaterial, Core::ResourceLoadRequest::create<Core::TypeLoader>(
									std::string{ "assets/materials/background_material.json" },
									std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>()));

		const auto backgroundSoundResource = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(
			backgroundSoundResource,
			Core::ResourceLoadRequest::create<Audio::SoundDescriptor>("./assets/sounds/background_crickets.ogg"));

		const auto backgroundRenderObject = mRegistry.create();
		mRegistry.emplace<Core::Spatial>(backgroundRenderObject, 1000.0f, 750.0f, 0.0f, 2.0f, 2.0f, 2.0f);
		mRegistry.emplace<Gfx::Sprite>(backgroundRenderObject);
		mRegistry.emplace<Gfx::RenderObject>(backgroundRenderObject, backgroundMaterial);
		mRegistry.emplace<Audio::AudioSource>(backgroundRenderObject, backgroundSoundResource);
		mRegistry.emplace<Audio::PlaySoundSourceRequest>(backgroundRenderObject, true);

		mBackgroundRenderObject = backgroundRenderObject;

		mSpawnedRenderObjects.reserve(100);

		const auto tilesMaterialResourceHandle = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(
			tilesMaterialResourceHandle,
			Core::ResourceLoadRequest::create<Core::TypeLoader>(
				"assets/materials/tiles.json", std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>()
			)
		);

		const auto floorEntity = mRegistry.create();
		mRegistry.emplace<Core::Spatial>(floorEntity, 650.0f, 450.0f, 1.0f, 10.0f, 1.0f, 1.0f);

		mRegistry.emplace<Gfx::RenderObject>(floorEntity, tilesMaterialResourceHandle);
		mRegistry.emplace<Gfx::Sprite>(floorEntity);

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
				cameraSpatial.x -= mTimer.getDeltaT() * 25.0f;
				if (cameraSpatial.x < -50.0f) {
					tickTock = false;
				}
			} else {
				cameraSpatial.x += mTimer.getDeltaT() * 25.0f;
				if (cameraSpatial.x > 50.0f) {
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

			auto clockNow = std::chrono::steady_clock::now();
			for (auto&& spawnedObject : mSpawnedRenderObjects) {
				auto& renderObject = mRegistry.get<Gfx::RenderObject>(spawnedObject);

				const bool canTickAnimation =
					(clockNow - renderObject.lastAnimUpdateTime) > std::chrono::milliseconds(500);
				if (canTickAnimation) {
					renderObject.currentSpriteFrame++;
					if (renderObject.currentSpriteFrame > 1) {
						renderObject.currentSpriteFrame = 0;
					}

					renderObject.lastAnimUpdateTime = clockNow;
				}
			}
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

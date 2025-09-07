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

namespace Game {

	Client::Client(
		Core::EnTTRegistry& registry, Core::Scheduler& scheduler, Core::CoreSystems&, Audio::AudioSystems&,
		Gfx::GfxSystems&, Input::InputSystems&, ScriptingJS::ScriptingJSSystems&, ScriptingLua::ScriptingLuaSystems&,
		DevTools::DevToolsSystems&, Core::Timer& timer)
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

		const auto catMaterialResource = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(
			catMaterialResource,
			Core::ResourceLoadRequest::create<Core::TypeLoader>(
				"assets/materials/cat.json", std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>()));

		const auto frogMaterialResource = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(
			frogMaterialResource, Core::ResourceLoadRequest::create<Core::TypeLoader>(
									  "assets/materials/frog.json",
									  std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>()));

		const auto catSoundResource = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(
			catSoundResource,
			Core::ResourceLoadRequest::create<Audio::SoundDescriptor>("./assets/sounds/cat_meow.wav"));

		const auto frogSoundResource = mRegistry.create();
		mRegistry.emplace<Core::ResourceLoadRequest>(
			frogSoundResource,
			Core::ResourceLoadRequest::create<Audio::SoundDescriptor>("./assets/sounds/frog_ribbit.ogg"));

		mTickHandle = scheduler.schedule([this, catMaterialResource, catSoundResource, frogMaterialResource,
										  frogSoundResource, lastSpawn = std::chrono::steady_clock::time_point{}, tickTock = false] mutable {

			Core::logEntry(mRegistry, "DeltaT: {}", mTimer.getDeltaT());

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

				const auto renderObjectEntity = mRegistry.create();
				mRegistry.emplace<Core::Spatial>(renderObjectEntity, mouseState.x, mouseState.y, spawnCat ? 2.0f : 1.0f, 8.0f, 8.0f);
				mRegistry.emplace<Gfx::RenderObject>(
					renderObjectEntity, spawnCat ? catMaterialResource : frogMaterialResource);
				mRegistry.emplace<Gfx::Sprite>(renderObjectEntity);
				mRegistry.emplace<Audio::AudioSource>(
					renderObjectEntity, spawnCat ? catSoundResource : frogSoundResource);
				mRegistry.emplace<Audio::PlaySoundSourceRequest>(renderObjectEntity, false, spawnCat ? 3.0f : 1.0f);

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

			const float groundY = 500.0f;
			const float gravity = -600.0f * mTimer.getDeltaT();

			for (auto&& spawnedObject : mSpawnedRenderObjects) {
				auto& spatial = mRegistry.get<Core::Spatial>(spawnedObject);

				const bool isOnGround = spatial.y >= groundY;
				if (!isOnGround) {
					spatial.y -= gravity;
				} else {
					if (inputX != 0.0f || inputY != 0.0f) {
						spatial.x += inputX;
						spatial.y += inputY;
					}
				}

				if (spatial.y > groundY) {
					spatial.y = groundY;
				}
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

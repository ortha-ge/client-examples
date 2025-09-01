module;

#include <chrono>
#include <random>

#include <entt/entt.hpp>

module Game.Client;

import Audio.AudioSource;
import Audio.SoundDescriptor;
import Audio.PlaySoundSourceRequest;
import Core.FileDescriptor;
import Core.FileLoadRequest;
import Core.JsonTypeLoaderAdapter;
import Core.Spatial;
import Core.TypeLoader;
import Core.Window;
import Gfx.MaterialDescriptor;
import Gfx.RenderObject;
import Input.KeyboardState;
import Input.MouseState;

namespace Game {

	Client::Client(Core::EnTTRegistry& registry,
		Core::Scheduler& scheduler,
		Core::CoreSystems&,
		Audio::AudioSystems&,
		Gfx::GfxSystems&,
		Input::InputSystems&,
		ScriptingJS::ScriptingJSSystems&,
		ScriptingLua::ScriptingLuaSystems&,
		DevTools::DevToolsSystems&
		)
		: mRegistry(registry)
		, mScheduler{ scheduler } {

		mWindowEntity = mRegistry.create();
		mRegistry.emplace<Core::Window>(mWindowEntity, "Riastradh", 1360, 768);

		const auto backgroundMaterial = mRegistry.create();
		mRegistry.emplace<Core::FileDescriptor>(backgroundMaterial, "assets/materials/background_material.json");
		mRegistry.emplace<Core::TypeLoader>(backgroundMaterial, std::make_unique<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());
		mRegistry.emplace<Core::FileLoadRequest>(backgroundMaterial);

		const auto backgroundSoundResource = mRegistry.create();
		mRegistry.emplace<Core::FileDescriptor>(backgroundSoundResource, "./assets/sounds/background_crickets.ogg");
		mRegistry.emplace<Audio::SoundDescriptor>(backgroundSoundResource);
		mRegistry.emplace<Core::FileLoadRequest>(backgroundSoundResource);

		const auto backgroundRenderObject = mRegistry.create();
		mRegistry.emplace<Core::Spatial>(backgroundRenderObject, 0.0f, 0.0f, 1.0f);
		mRegistry.emplace<Gfx::RenderObject>(backgroundRenderObject, backgroundMaterial);
		mRegistry.emplace<Audio::AudioSource>(backgroundRenderObject, backgroundSoundResource);
		mRegistry.emplace<Audio::PlaySoundSourceRequest>(backgroundRenderObject, true);

		mBackgroundRenderObject = backgroundRenderObject;

		mSpawnedRenderObjects.reserve(100);

		// const auto materialResource = mRegistry.create();
		// mRegistry.emplace<Core::FileDescriptor>(materialResource, "assets/materials/frog_material.json");
		// mRegistry.emplace<Core::TypeLoader>(materialResource, std::make_unique<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());
		// mRegistry.emplace<Core::FileLoadRequest>(materialResource);
		//
		// const auto soundResource = mRegistry.create();
		// mRegistry.emplace<Core::FileDescriptor>(soundResource, "./assets/sounds/frog_ribbit.ogg");
		// mRegistry.emplace<Audio::SoundDescriptor>(soundResource);
		// mRegistry.emplace<Core::FileLoadRequest>(soundResource);

		mTickHandle = scheduler.schedule([this/*, materialResource, soundResource*/, lastSpawn = std::chrono::steady_clock::time_point{}] mutable {
			if (!mRegistry.all_of<Input::KeyboardState, Input::MouseState>(mWindowEntity)) {
				return;
			}

			const auto& mouseState = mRegistry.get<Input::MouseState>(mWindowEntity);
			const bool canSpawn = (std::chrono::steady_clock::now() - lastSpawn) > std::chrono::milliseconds(150);
			if (canSpawn && Input::isMouseButtonPressed(mouseState, Input::MouseButton::Left)) {
				lastSpawn = std::chrono::steady_clock::now();

				// const auto renderObjectEntity = mRegistry.create();
				// mRegistry.emplace<Core::Spatial>(renderObjectEntity, mouseState.x, mouseState.y, 0.0f);
				// mRegistry.emplace<Gfx::RenderObject>(renderObjectEntity, materialResource);
				// mRegistry.emplace<Audio::AudioSource>(renderObjectEntity, soundResource);
				// mRegistry.emplace<Audio::PlaySoundSourceRequest>(renderObjectEntity);
				//
				// mSpawnedRenderObjects.push_back(renderObjectEntity);
			}

			const auto& keyboardState = mRegistry.get<Input::KeyboardState>(mWindowEntity);

			float inputX = 0.0f;
			float inputY = 0.0f;

			if (Input::isKeyPressed(keyboardState, Input::Key::Left)) {
				inputX -= 0.1f;
			} else if (Input::isKeyPressed(keyboardState, Input::Key::Right)) {
				inputX += 0.1f;
			}

			if (Input::isKeyPressed(keyboardState, Input::Key::Up)) {
				inputY -= 0.1f;
			} else if (Input::isKeyPressed(keyboardState, Input::Key::Down)) {
				inputY += 0.1f;
			}

			if (inputX != 0.0f || inputY != 0.0f) {
				for (auto&& spawnedObject : mSpawnedRenderObjects) {
					auto& spatial = mRegistry.get<Core::Spatial>(spawnedObject);
					spatial.x += inputX;
					spatial.y += inputY;
				}
			}

			const auto clockNow = std::chrono::steady_clock::now();
			for (auto&& spawnedObject : mSpawnedRenderObjects) {
				auto& renderObject = mRegistry.get<Gfx::RenderObject>(spawnedObject);

				const bool canTickAnimation = (clockNow - renderObject.lastAnimUpdateTime) > std::chrono::milliseconds(500);
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

} // Game
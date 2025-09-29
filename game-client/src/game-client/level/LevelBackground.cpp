module;

#include <memory>

module Game.LevelBackground;

import Audio.AudioSource;
import Audio.PlaySoundSourceRequest;
import Audio.SoundDescriptor;
import Core.EnTTNode;
import Core.JsonTypeLoaderAdapter;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TypeLoader;
import Gfx.RenderObject;
import Gfx.SpriteObject;
import Gfx.Reflection.MaterialDescriptor;
import Gfx.Reflection.Sprite;
import entt;
import glm;

namespace Game {

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

} // namespace Game

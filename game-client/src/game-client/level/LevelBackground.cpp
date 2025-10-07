module;

#include <memory>

module Game.LevelBackground;

import Ortha.Audio.AudioSource;
import Ortha.Audio.PlaySoundSourceRequest;
import Ortha.Audio.SoundDescriptor;
import Ortha.Core.EnTTNode;
import Ortha.Core.JsonTypeLoaderAdapter;
import Ortha.Core.ResourceLoadRequest;
import Ortha.Core.Spatial;
import Ortha.Core.TypeLoader;
import Ortha.Gfx.RenderObject;
import Ortha.Gfx.SpriteObject;
import Ortha.Gfx.Reflection.MaterialDescriptor;
import Ortha.Gfx.Reflection.Sprite;
import entt;
import glm;

namespace Game {

	entt::entity createBackground(entt::registry& registry) {
		using namespace Ortha::Audio;
		using namespace Core;
		using namespace Ortha::Gfx;

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

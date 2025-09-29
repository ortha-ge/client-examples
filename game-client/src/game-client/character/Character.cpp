module;

#include <format>
#include <memory>

module Game.Character;

import Audio.AudioSource;
import Audio.PlaySoundSourceRequest;
import Audio.SoundDescriptor;
import Core.EnTTNode;
import Core.JsonTypeLoaderAdapter;
import Core.Node;
import Core.ResourceLoadRequest;
import Core.Scheduler;
import Core.Spatial;
import Core.Timer;
import Core.TypeLoader;
import Game.CharacterController;
import Gfx.MaterialDescriptor;
import Gfx.Reflection.MaterialDescriptor;
import Gfx.Reflection.Sprite;
import Gfx.RenderObject;
import Gfx.SpriteDescriptor;
import Gfx.SpriteObject;
import Physics2d.CollisionShape;
import Physics2d.Rigidbody2d;
import Physics2d.Reflection.BoxCollisionShape;
import Physics2d.Reflection.CircleCollisionShape;
import entt;
import glm;

namespace Game {

	entt::entity createCharacter(
		entt::registry& registry, const CharacterConfig& characterConfig, glm::vec3 pos, glm::vec3 scale,
		float soundRequestVolume) {
		using namespace Audio;
		using namespace Core;
		using namespace Gfx;
		using namespace Physics2d;

		static size_t characterCounter{ 0zu };
		++characterCounter;

		const auto materialResource = ResourceLoadRequest::create<TypeLoader>(
			registry, characterConfig.materialFilePath,
			std::make_shared<JsonTypeLoaderAdapter<MaterialDescriptor>>());

		const auto spriteResource = ResourceLoadRequest::create<TypeLoader>(
			registry, characterConfig.spriteFilePath, std::make_shared<JsonTypeLoaderAdapter<SpriteDescriptor>>());

		const auto soundResource =
			ResourceLoadRequest::create<SoundDescriptor>(registry, characterConfig.spawnSoundFilePath);

		const auto collisionShapeResource = ResourceLoadRequest::create<TypeLoader>(registry,
			characterConfig.collisionShapeFilePath,
			std::make_shared<JsonTypeLoaderAdapter<CollisionShapeDescriptor>>()
		);

		const auto characterEntity = createEnTTNode(registry, std::format("{}_{}",  characterConfig.name, characterCounter));
		registry.emplace<Character>(characterEntity, Character {
			.tag = characterConfig.tag,
			.jumpImpulse = characterConfig.jumpImpulse,
			.moveImpulse = characterConfig.moveImpulse
		});
		registry.emplace<Spatial>(characterEntity, pos, scale);
		registry.emplace<CollisionShape>(characterEntity, collisionShapeResource);
		registry.emplace<Rigidbody>(characterEntity, false);
		registry.emplace<AudioSource>(characterEntity, soundResource);
		registry.emplace<PlaySoundSourceRequest>(characterEntity, soundRequestVolume);

		const auto renderObjectEntity = createChildEnTTNode(registry, characterEntity, "RenderObject");
		registry.emplace<Spatial>(renderObjectEntity);
		registry.emplace<RenderObject>(renderObjectEntity, materialResource);
		registry.emplace<SpriteObject>(renderObjectEntity, spriteResource);

		return characterEntity;
	}

} // namespace Game
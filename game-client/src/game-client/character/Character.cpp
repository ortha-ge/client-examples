module;

#include <format>
#include <memory>

module Game.Character;

import Ortha.Audio.AudioSource;
import Ortha.Audio.PlaySoundSourceRequest;
import Ortha.Audio.SoundDescriptor;
import Ortha.Core.EnTTNode;
import Ortha.Core.JsonTypeLoaderAdapter;
import Ortha.Core.Node;
import Ortha.Core.ResourceLoadRequest;
import Ortha.Core.Scheduler;
import Ortha.Core.Spatial;
import Ortha.Core.Timer;
import Ortha.Core.TypeLoader;
import Game.CharacterController;
import Ortha.Gfx.MaterialDescriptor;
import Ortha.Gfx.Reflection.MaterialDescriptor;
import Ortha.Gfx.Reflection.Sprite;
import Ortha.Gfx.RenderObject;
import Ortha.Gfx.SpriteDescriptor;
import Ortha.Gfx.SpriteObject;
import Ortha.Physics2d.CollisionShape;
import Ortha.Physics2d.Rigidbody2d;
import Ortha.Physics2d.Reflection.BoxCollisionShape;
import Ortha.Physics2d.Reflection.CircleCollisionShape;
import entt;
import glm;

namespace Game {

	entt::entity createCharacter(
		entt::registry& registry, const CharacterConfig& characterConfig, glm::vec3 pos, glm::vec3 scale,
		float soundRequestVolume) {
		using namespace Ortha::Audio;
		using namespace Ortha::Core;
		using namespace Ortha::Gfx;
		using namespace Ortha::Physics2d;

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
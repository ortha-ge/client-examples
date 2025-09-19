module;

#include <format>
#include <memory>

#include <entt/entt.hpp>
#include <glm/vec3.hpp>

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
import Core.TemplateNode;
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

namespace Game {

	entt::entity createCharacter(entt::registry& registry, const CharacterResourceConfig& resourceConfig,
		Core::Scheduler& scheduler, Core::Timer& timer, glm::vec3 pos, glm::vec3 scale, float soundRequestVolume) {
		using namespace Audio;
		using namespace Core;
		using namespace Gfx;
		using namespace Physics2d;

		static size_t characterCounter{ 0zu };
		++characterCounter;

		const auto materialResource = ResourceLoadRequest::create<TypeLoader>(
			registry, resourceConfig.materialFilePath,
			std::make_shared<JsonTypeLoaderAdapter<MaterialDescriptor>>());

		const auto spriteResource = ResourceLoadRequest::create<TypeLoader>(
			registry, resourceConfig.spriteFilePath, std::make_shared<JsonTypeLoaderAdapter<SpriteDescriptor>>());

		const auto soundResource =
			ResourceLoadRequest::create<SoundDescriptor>(registry, resourceConfig.spawnSoundFilePath);

		const auto characterRootNodeEntity = registry.create();
		registry.emplace<Spatial>(characterRootNodeEntity, pos, scale);

		const auto collisionShapeResource = ResourceLoadRequest::create<TypeLoader>(registry,
			resourceConfig.collisionShapeFilePath,
			std::make_shared<JsonTypeLoaderAdapter<CollisionShapeDescriptor>>()
		);

		registry.emplace<CollisionShape>(characterRootNodeEntity, collisionShapeResource);
		registry.emplace<Rigidbody>(characterRootNodeEntity, false);
		registry.emplace<AudioSource>(characterRootNodeEntity, soundResource);
		registry.emplace<PlaySoundSourceRequest>(characterRootNodeEntity, soundRequestVolume);

		entt::handle characterRootHandle(registry, characterRootNodeEntity);
		NodeHandle& characterRootNode{ registry.emplace<NodeHandle>(characterRootNodeEntity, NodeHandle::create<EnTTNode>(std::format("Character_{}", characterCounter), characterRootHandle)) };

		const auto renderObjectEntity = registry.create();
		registry.emplace<Spatial>(renderObjectEntity);

		registry.emplace<RenderObject>(renderObjectEntity, materialResource);
		registry.emplace<SpriteObject>(renderObjectEntity, spriteResource);

		auto& childNodeHandle = registry.emplace<NodeHandle>(renderObjectEntity, NodeHandle::create<EnTTNode>("RenderObject", entt::handle(registry, renderObjectEntity)));
		characterRootNode.getNode()->addChild(childNodeHandle.getNode());

		return characterRootNodeEntity;
	}

} // namespace Game
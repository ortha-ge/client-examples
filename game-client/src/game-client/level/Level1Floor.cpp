module;

#include <memory>

module Game.Level1Floor;

import Ortha.Core.EnTTNode;
import Ortha.Core.JsonTypeLoaderAdapter;
import Ortha.Core.ResourceLoadRequest;
import Ortha.Core.Spatial;
import Ortha.Core.TypeLoader;
import Ortha.Gfx.TilemapObject;
import Ortha.Gfx.Reflection.ShaderProgramDescriptor;
import Ortha.Gfx.Reflection.Tilemap;
import Ortha.Physics2d.BoxCollisionShape;
import Ortha.Physics2d.CollisionShape;
import Ortha.Physics2d.Rigidbody2d;
import entt;
import glm;

namespace Game {

	entt::entity createLevel1Floor(entt::registry& registry) {
		using namespace Ortha::Core;
		using namespace Ortha::Physics2d;
		using namespace Ortha::Gfx;

		auto tilemapTypeLoader = std::make_shared<JsonTypeLoaderAdapter<TilemapDescriptor>>();
		auto levelTileMap = ResourceLoadRequest::create<TypeLoader>(registry, "assets/tilemaps/level1.json", tilemapTypeLoader);

		auto shaderProgramTypeLoader = std::make_shared<JsonTypeLoaderAdapter<ShaderProgramDescriptor>>();
		auto shaderProgram = ResourceLoadRequest::create<TypeLoader>(registry, "assets/shaders/sprite_shader_program.json", shaderProgramTypeLoader);

		// Base Plate
		const auto floorEntity = createEnTTNode(registry, "Floor");
		registry.emplace<Spatial>(floorEntity, glm::vec3{ 650.0f, 750.0f, 10.0f }, glm::vec3{ 20.0f, 1.0f, 1.0f });

		BoxCollisionShape boxCollisionDescriptor{
			100.0f,
			100.0f,
		};
		registry.emplace<Ortha::Physics2d::CollisionShape>(floorEntity, boxCollisionDescriptor);
		registry.emplace<Ortha::Physics2d::Rigidbody>(floorEntity);

		// Left end piece
		const auto leftWallColliderEntity = createEnTTNode(registry, "FloorLeftWall");
		registry.emplace<Spatial>(leftWallColliderEntity, glm::vec3{ -300.0f, 650.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
		registry.emplace<Ortha::Physics2d::CollisionShape>(leftWallColliderEntity, boxCollisionDescriptor);
		registry.emplace<Ortha::Physics2d::Rigidbody>(leftWallColliderEntity);

		const auto rightWallColliderEntity = createEnTTNode(registry, "FloorRightWall");
		registry.emplace<Spatial>(rightWallColliderEntity, glm::vec3{ 1600.0f, 650.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
		registry.emplace<Ortha::Physics2d::CollisionShape>(rightWallColliderEntity, boxCollisionDescriptor);
		registry.emplace<Ortha::Physics2d::Rigidbody>(rightWallColliderEntity);

		const auto floorTilemapObject = createChildEnTTNode(registry, floorEntity, "FloorTilemap");
		registry.emplace<Spatial>(floorTilemapObject, glm::vec3{ -1000.0f, -125.0f, 0.0f });
		registry.emplace<TilemapObject>(floorTilemapObject, levelTileMap, shaderProgram);

		return floorEntity;
	}

} // namespace Game

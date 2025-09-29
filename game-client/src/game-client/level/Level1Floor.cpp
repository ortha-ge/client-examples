module;

#include <memory>

module Game.Level1Floor;

import Core.EnTTNode;
import Core.JsonTypeLoaderAdapter;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TypeLoader;
import Gfx.TilemapObject;
import Gfx.Reflection.ShaderProgramDescriptor;
import Gfx.Reflection.Tilemap;
import Physics2d.BoxCollisionShape;
import Physics2d.CollisionShape;
import Physics2d.Rigidbody2d;
import entt;
import glm;

namespace Game {

	entt::entity createLevel1Floor(entt::registry& registry) {
		using namespace Core;
		using namespace Gfx;

		auto tilemapTypeLoader = std::make_shared<JsonTypeLoaderAdapter<TilemapDescriptor>>();
		auto levelTileMap = ResourceLoadRequest::create<TypeLoader>(registry, "assets/tilemaps/level1.json", tilemapTypeLoader);

		auto shaderProgramTypeLoader = std::make_shared<JsonTypeLoaderAdapter<ShaderProgramDescriptor>>();
		auto shaderProgram = ResourceLoadRequest::create<TypeLoader>(registry, "assets/shaders/sprite_shader_program.json", shaderProgramTypeLoader);

		// Base Plate
		const auto floorEntity = createEnTTNode(registry, "Floor");
		registry.emplace<Spatial>(floorEntity, glm::vec3{ 650.0f, 750.0f, 10.0f }, glm::vec3{ 20.0f, 1.0f, 1.0f });

		Physics2d::BoxCollisionShape boxCollisionDescriptor{
			100.0f,
			100.0f,
		};
		registry.emplace<Physics2d::CollisionShape>(floorEntity, boxCollisionDescriptor);
		registry.emplace<Physics2d::Rigidbody>(floorEntity);

		// Left end piece
		const auto leftWallColliderEntity = createEnTTNode(registry, "FloorLeftWall");
		registry.emplace<Spatial>(leftWallColliderEntity, glm::vec3{ -300.0f, 650.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
		registry.emplace<Physics2d::CollisionShape>(leftWallColliderEntity, boxCollisionDescriptor);
		registry.emplace<Physics2d::Rigidbody>(leftWallColliderEntity);

		const auto rightWallColliderEntity = createEnTTNode(registry, "FloorRightWall");
		registry.emplace<Spatial>(rightWallColliderEntity, glm::vec3{ 1600.0f, 650.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
		registry.emplace<Physics2d::CollisionShape>(rightWallColliderEntity, boxCollisionDescriptor);
		registry.emplace<Physics2d::Rigidbody>(rightWallColliderEntity);

		const auto floorTilemapObject = createChildEnTTNode(registry, floorEntity, "FloorTilemap");
		registry.emplace<Spatial>(floorTilemapObject, glm::vec3{ -1000.0f, -125.0f, 0.0f });
		registry.emplace<TilemapObject>(floorTilemapObject, levelTileMap, shaderProgram);

		return floorEntity;
	}

} // namespace Game

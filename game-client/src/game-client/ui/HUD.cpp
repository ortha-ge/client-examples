module;

#include <entt/entt.hpp>

#include <glm/vec3.hpp>

module Game.HUD;


import Core.EnTTNode;
import Core.Node;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Gfx.FontDescriptor;
import Gfx.FontObject;


namespace Game {

	const entt::entity createHUD(entt::registry& registry) {
		using namespace Core;

		const entt::entity hudEntity = registry.create();
		registry.emplace<HUD>(hudEntity);
		registry.emplace<Spatial>(hudEntity);
		registry.emplace<NodeHandle>(hudEntity, NodeHandle::create<EnTTNode>("HUD", entt::handle(registry, hudEntity)));

		auto fontResource = ResourceLoadRequest::create<Gfx::FontDescriptor>(registry, "assets/fonts/roboto_regular.ttf");

		const entt::entity scoreTextEntity{ registry.create() };
		registry.emplace<Spatial>(scoreTextEntity, glm::vec3{ 200.0f, 100.0f, 0.0f });
		registry.emplace<Gfx::FontObject>(scoreTextEntity,
			R"(She said, "Hey y'all!"__  __  __gap
Some text on a new line...
uVvWwXxYyZz1234567890!.,/\
"'[]{}-+()*&^%$#@)", fontResource);
		registry.emplace<NodeHandle>(scoreTextEntity, NodeHandle::create<EnTTNode>("ScoreText", entt::handle(registry, scoreTextEntity)));

		addChildNode(registry, hudEntity, scoreTextEntity);

		return hudEntity;
	}

} // namespace Game

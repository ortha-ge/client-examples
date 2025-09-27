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

	const entt::entity createHUD(entt::registry& registry, const entt::entity playerEntity) {
		using namespace Core;
		using namespace Gfx;

		const entt::entity hudEntity = createEnTTNode(registry, "HUD");
		registry.emplace<HUD>(hudEntity, playerEntity);
		registry.emplace<Spatial>(hudEntity);

		auto fontResource = ResourceLoadRequest::create<FontDescriptor>(registry, "assets/fonts/roboto_regular.ttf");
		const entt::entity scoreTextEntity{ createChildEnTTNode(registry, hudEntity, "ScoreText") };
		registry.emplace<Spatial>(scoreTextEntity, glm::vec3{ 200.0f, 100.0f, 0.0f });
		registry.emplace<FontObject>(scoreTextEntity, "Score ---", fontResource);

		return hudEntity;
	}

} // namespace Game

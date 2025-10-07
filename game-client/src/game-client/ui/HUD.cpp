module Game.HUD;

import Ortha.Core.EnTTNode;
import Ortha.Core.Node;
import Ortha.Core.ResourceLoadRequest;
import Ortha.Core.Spatial;
import Ortha.Gfx.FontDescriptor;
import Ortha.Gfx.FontObject;
import glm;

namespace Game {

	const entt::entity createHUD(entt::registry& registry, const entt::entity playerEntity) {
		using namespace Ortha::Core;
		using namespace Ortha::Gfx;

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

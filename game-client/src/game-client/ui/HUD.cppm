export module Game.HUD;

import entt;

export namespace Game {

	const entt::entity createHUD(entt::registry&, entt::entity);

	struct HUD {
		entt::entity player{ entt::null };
	};

} // namespace Game

module;

#include <cstdint>

export module Game.PlayerScore;

export namespace Game {

	struct PlayerScore {
		uint64_t score{ 0u };
	};

} // namespace Game

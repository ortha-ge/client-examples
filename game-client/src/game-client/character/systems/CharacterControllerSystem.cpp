module;

#include <utility>

module Game.CharacterControllerSystem;

import Ortha.Audio.PlaySoundSourceRequest;
import Game.Character;
import Game.CharacterController;
import Ortha.Input.KeyboardEvent;
import Ortha.Input.KeyboardState;
import Ortha.Physics2d.ApplyForceRequest;
import Ortha.Physics2d.Rigidbody2d;

namespace Game {

	CharacterControllerSystem::CharacterControllerSystem(entt::registry& registry, Ortha::Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	CharacterControllerSystem::~CharacterControllerSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void CharacterControllerSystem::tickSystem(entt::registry& registry) {
		using namespace Ortha::Audio;
		using namespace Ortha::Input;
		using namespace Ortha::Physics2d;

		float inputX = 0.0f;
		float inputY = 0.0f;

		registry.view<KeyboardEvent>()
			.each([&inputY](const KeyboardEvent& keyboardEvent) {
				if (keyboardEvent.key == Key::Space &&
					keyboardEvent.eventType == InputEventType::Pressed) {

					inputY -= 1.0f;
				}
			});

		registry.view<KeyboardState>()
			.each([&inputX](const KeyboardState& keyboardState) {
				if (isKeyPressed(keyboardState, Key::Left)) {
					inputX -= 1.0f;
				} else if (isKeyPressed(keyboardState, Key::Right)) {
					inputX += 1.0f;
				}
			});

		if (inputX != 0.0f || inputY != 0.0f) {
			registry.view<Character, CharacterController, Rigidbody>()
				.each([&registry, inputX, inputY](const entt::entity entity, const Character& character, Rigidbody& rigidBody) {
					ApplyForceRequest applyForceRequest;
					applyForceRequest.type = ForceType::Impulse;
					applyForceRequest.force.x = inputX * character.moveImpulse;

					if (!rigidBody.isOnGround) {
						applyForceRequest.force.x *= 0.1f;
					}

					if (rigidBody.isOnGround && inputY != 0.0f) {
						applyForceRequest.force.y = inputY * character.jumpImpulse;

						if (!registry.all_of<PlaySoundSourceRequest>(entity)) {
							PlaySoundSourceRequest playSoundRequest;
							playSoundRequest.volumeScale = 3.0f;
							registry.emplace<PlaySoundSourceRequest>(entity, playSoundRequest);
						}
					}

					registry.get_or_emplace<ApplyForceRequest>(entity) = std::move(applyForceRequest);
				});
		}
	}

} // namespace Game

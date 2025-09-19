module;

#include <utility>

#include <entt/entt.hpp>

module Game.CharacterControllerSystem;

import Audio.PlaySoundSourceRequest;
import Game.Character;
import Game.CharacterController;
import Input.KeyboardState;
import Physics2d.ApplyForceRequest;
import Physics2d.Rigidbody2d;

namespace Game {

	CharacterControllerSystem::CharacterControllerSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
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
		using namespace Physics2d;

		float inputX = 0.0f;
		float inputY = 0.0f;

		registry.view<Input::KeyboardState>()
			.each([&inputX, &inputY](const Input::KeyboardState& keyboardState) {
				if (Input::isKeyPressed(keyboardState, Input::Key::Left)) {
					inputX -= 1.0f;
				} else if (Input::isKeyPressed(keyboardState, Input::Key::Right)) {
					inputX += 1.0f;
				}

				if (Input::isKeyPressed(keyboardState, Input::Key::Up)) {
					inputY -= 1.0f;
				} else if (Input::isKeyPressed(keyboardState, Input::Key::Down)) {
				}
			});

		if (inputX != 0.0f || inputY != 0.0f) {
			registry.view<Character, CharacterController, Rigidbody>()
				.each([&registry, inputX, inputY](const entt::entity entity, const Character& character, Rigidbody& rigidBody) {
					ApplyForceRequest applyForceRequest;
					applyForceRequest.type = ForceType::Impulse;
					applyForceRequest.force.x = inputX * character.moveImpulse;

					if (rigidBody.isOnGround && inputY != 0.0f) {
						applyForceRequest.force.y = inputY * character.jumpImpulse;
						applyForceRequest.force.x *= 0.5f;

						if (!registry.all_of<Audio::PlaySoundSourceRequest>(entity)) {
							Audio::PlaySoundSourceRequest playSoundRequest;
							playSoundRequest.volumeScale = 3.0f;
							registry.emplace<Audio::PlaySoundSourceRequest>(entity, playSoundRequest);
						}
					}

					registry.get_or_emplace<ApplyForceRequest>(entity) = std::move(applyForceRequest);
				});
		}
	}

} // namespace Game

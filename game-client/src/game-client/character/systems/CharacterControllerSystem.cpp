module;

#include <utility>

#include <entt/entt.hpp>

module Game.CharacterControllerSystem;

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

		constexpr float jumpSpeed = 100000.0f;
		constexpr float moveSpeed = 200.0f;
		float inputX = 0.0f;
		float inputY = 0.0f;

		registry.view<Input::KeyboardState>()
			.each([&inputX, &inputY](const Input::KeyboardState& keyboardState) {
				if (Input::isKeyPressed(keyboardState, Input::Key::Left)) {
					inputX -= moveSpeed;
				} else if (Input::isKeyPressed(keyboardState, Input::Key::Right)) {
					inputX += moveSpeed;
				}

				if (Input::isKeyPressed(keyboardState, Input::Key::Up)) {
					inputY -= jumpSpeed;
				} else if (Input::isKeyPressed(keyboardState, Input::Key::Down)) {
				}
			});

		registry.view<CharacterController, Rigidbody>()
			.each([&registry, inputX, inputY](const entt::entity entity, Rigidbody& rigidBody) {
				ApplyForceRequest applyForceRequest;
				applyForceRequest.type = ForceType::Impulse;
				applyForceRequest.force.x = inputX;

				if (rigidBody.isOnGround) {
					applyForceRequest.force.y = inputY;
				}

				registry.get_or_emplace<ApplyForceRequest>(entity) = std::move(applyForceRequest);
			});
	}

} // namespace Game

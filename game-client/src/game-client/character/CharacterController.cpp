module;

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

module Game.CharacterController;

import Core.EnTTNode;
import Core.Node;
import Core.TypeId;
import Input.KeyboardState;
import Physics2d.ApplyForceRequest;
import Physics2d.Rigidbody2d;

namespace Game {

	CharacterController::CharacterController(Core::Scheduler& scheduler, Core::Timer& timer, entt::handle characterHandle)
		: mScheduler(scheduler)
		, mTimer(timer)
		, mCharacterHandle(characterHandle){

		mTickHandle = mScheduler.schedule([this] {
			tick();
		});
	}

	CharacterController::~CharacterController() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void CharacterController::tick() {
		if (!mCharacterHandle.valid()) {
			return;
		}

		constexpr float jumpSpeed = 100000.0f;
		constexpr float moveSpeed = 200.0f;
		float inputX = 0.0f;
		float inputY = 0.0f;

		auto& registry{ *mCharacterHandle.registry() };
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

		Physics2d::ApplyForceRequest applyForceRequest;
		applyForceRequest.type = Physics2d::ForceType::Impulse;
		applyForceRequest.force.x = inputX;

		auto& rigidBody{ mCharacterHandle.get<Physics2d::Rigidbody>() };
		if (rigidBody.isOnGround) {
			applyForceRequest.force.y = inputY;
		}

		mCharacterHandle.get_or_emplace<Physics2d::ApplyForceRequest>() = applyForceRequest;
	}

} // namespace Game

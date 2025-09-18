module;

#include <utility>

#include <entt/entt.hpp>

module Game.CameraController;

import Core.Spatial;

namespace Game {

	CameraController::CameraController(Core::Scheduler& scheduler, Core::Timer& timer, entt::handle cameraHandle)
		: mScheduler(scheduler)
		, mTimer(timer)
		, mCameraHandle(cameraHandle) {

		mTickHandle = mScheduler.schedule([this] {
			tick();
		});
	}

	CameraController::~CameraController() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void CameraController::tick() {
		if (!mCameraHandle.valid()) {
			return;
		}

		auto& spatial{ mCameraHandle.get<Core::Spatial>() };

		if (mTickTock) {
			spatial.position.x -= mTimer.getDeltaT() * 25.0f;
			if (spatial.position.x < -50.0f) {
				mTickTock = false;
			}
		} else {
			spatial.position.x += mTimer.getDeltaT() * 25.0f;
			if (spatial.position.x > 50.0f) {
				mTickTock = true;
			}
		}
	}

} // namespace Game

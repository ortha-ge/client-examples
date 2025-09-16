
#include <filesystem>

#include <entt/entt.hpp>
#include <tracy/Tracy.hpp>

import kangaru;
import Audio.SystemsService;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Core.SystemsService;
import Core.QuitAppRequest;
import Game.ClientService;

static kgr::container* sContainer = nullptr;

#ifndef __EMSCRIPTEN__
static bool sMainLoopCanceled = false;
#endif

void stopMainLoop() {
#ifndef __EMSCRIPTEN__
	sMainLoopCanceled = true;
#else
	emscripten_cancel_main_loop();
#endif
}

void mainLoop() {
	if (!sContainer) {
		stopMainLoop();
		return;
	}

	kgr::container& container{ *sContainer };

	Core::Scheduler& scheduler = container.service<Core::SchedulerService>();
	entt::registry& registry = container.service<Core::EnTTRegistryService>();

	auto quitAppRequests = registry.view<const Core::QuitAppRequest>();
	if (!quitAppRequests.empty()) {
		stopMainLoop();
		return;
	}

	scheduler.tick();
}

void startMainLoop() {
#ifndef __EMSCRIPTEN__
	while (!sMainLoopCanceled) {
		mainLoop();
	}
#else
	emscripten_set_main_loop(mainLoop, 60, false);
#endif
}

int main(int argc, char* argv[]) {
	TracyNoop;

	std::filesystem::path exePath = argv[0];
	std::filesystem::current_path(exePath.parent_path());

	kgr::container container;
	container.service<Game::ClientService>();

	entt::registry& registry = container.service<Core::EnTTRegistryService>();
	auto quitAppRequests = registry.view<const Core::QuitAppRequest>();

	sContainer = &container;

	startMainLoop();

	quitAppRequests.each([&registry](entt::entity entity, const Core::QuitAppRequest&) { registry.destroy(entity); });

	return 0;
}

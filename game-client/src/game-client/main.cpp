
#include <filesystem>

#include <entt/entt.hpp>

import kangaru;
import Audio.SystemsService;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Core.SystemsService;
import Core.QuitAppRequest;
import Game.ClientService;

int main(int argc, char* argv[]) {
	std::filesystem::path exePath = argv[0];

	std::filesystem::current_path(exePath.parent_path());
	printf("CWD: %s\n", std::filesystem::current_path().c_str());

	kgr::container container;
	Core::Scheduler& scheduler = container.service<Core::SchedulerService>();
	entt::registry& registry = container.service<Core::EnTTRegistryService>();

	container.service<Game::ClientService>();

	auto quitAppRequests = registry.view<const Core::QuitAppRequest>();
	while(quitAppRequests.empty()) {
		scheduler.tick();
	}

	quitAppRequests.each([&registry](entt::entity entity, const Core::QuitAppRequest&) { registry.destroy(entity); });

	return 0;
} 

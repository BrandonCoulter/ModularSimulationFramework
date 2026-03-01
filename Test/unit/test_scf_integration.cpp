#include <cassert>
#include <cmath>
#include <cstdlib>
#include <string>

#include "Clock.hpp"
#include "EntityRegistry.hpp"
#include "SCF.hpp"
#include "Scheduler.hpp"

namespace {

std::string build_scenario_path() {
    const char* source_root = std::getenv("MSF_SOURCE_ROOT");
    assert(source_root != nullptr);
    return std::string(source_root) + "/Test/scenario/basic.xml";
}

} // namespace

int main() {
    EntityRegistry registry;
    registry.register_classes();

    SCF scf;
    scf.set_scf_filepath(build_scenario_path());

    msf::SimDt timestep = 0.0;
    const bool parse_ok = scf.parse_scf(registry, timestep);
    assert(parse_ok);

    assert(std::fabs(timestep - 0.001) < 1e-12);
    assert(registry.get_entity_count() == 3);

    SimulationClock clock;
    clock.reset(0.0);

    SimEventScheduler scheduler;

    int pending_request_count = 0;
    int fired_events = 0;

    registry.for_each_entity([&](Entity& entity) {
        for (auto& event_request : entity.pending_events) {
            ++pending_request_count;
            event_request.callback = [&fired_events]() {
                ++fired_events;
            };
        }
    });

    assert(pending_request_count == 2);

    registry.schedule_entitiy_events(scheduler, clock);

    int pending_after_schedule = 0;
    registry.for_each_entity([&](Entity& entity) {
        pending_after_schedule += static_cast<int>(entity.pending_events.size());
    });
    assert(pending_after_schedule == 0);

    while (clock.now() < 50.0) {
        scheduler.process_events(clock);
        clock.advance(timestep);
    }
    scheduler.process_events(clock);

    assert(fired_events == pending_request_count);
    assert(clock.now() >= 50.0);

    return 0;
}

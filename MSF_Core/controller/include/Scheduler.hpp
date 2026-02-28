// Event Scheduler class definition

#pragma once
#include <queue>
#include <functional>
#include "Clock.hpp"


class SimEventScheduler {
public:
    // Schedule an event to be executed after a certain delay (in seconds of sim time)
    void schedule_event(SimulationClock& clock, std::function<void()> event, SimulationClock::SimDt delay_seconds) {
        SimulationClock::SimTime execution_time = clock.now() + delay_seconds;
        event_queue.push({execution_time, event});
        std::cout << "[DEBUG] Scheduling event for sim time: " << execution_time << "s (now: " << clock.now() 
                  << "s, delay: " << delay_seconds << "s), queue size: " << event_queue.size() << std::endl;
    }
    // Process events that are due for execution
    void process_events(SimulationClock& clock) {
        SimulationClock::SimTime current_time = clock.now();
        while (!event_queue.empty() && event_queue.top().execution_time <= current_time) {
            auto event = event_queue.top().event;
            event_queue.pop();
            event(); // Execute the event
        }
    }
private:
    struct ScheduledEvent {
        SimulationClock::SimTime execution_time; // Absolute sim time (seconds) when event should execute
        std::function<void()> event; // The event to execute
        // Comparator for priority queue (earliest execution time has highest priority)
        bool operator<(const ScheduledEvent& other) const {
            return execution_time > other.execution_time; // Min-heap based on execution time
        }
    };
    std::priority_queue<ScheduledEvent> event_queue; // Priority queue to manage scheduled events
};
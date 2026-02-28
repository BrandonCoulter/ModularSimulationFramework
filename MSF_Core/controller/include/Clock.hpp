// Main Simulation Clock Definition

#pragma once

#include <chrono>
#include <cstdint>

// SimulationClock
//  - Tracks deterministic *simulation time* in seconds (SimTime).
//  - Wall clock is available for optional real-time pacing / diagnostics.
//  - Simulation time ONLY advances when advance(dt) is called and the clock is not paused.
class SimulationClock {
public:
    using WallClock = std::chrono::steady_clock;
    using WallTimePoint = WallClock::time_point;

    // Simulation time is represented as seconds.
    using SimTime = double; // seconds
    using SimDt   = double; // seconds

    SimulationClock() {
        reset(0.0);
    }

    // Reset sim time to a specific start time (seconds) and reset wall timing.
    void reset(SimTime start_time_seconds = 0.0) {
        sim_time_seconds_ = start_time_seconds;
        is_sim_time_paused_ = false;

        wall_clock_start_time_ = WallClock::now();
        last_wall_tick_time_ = wall_clock_start_time_;
    }

    // ------------------------
    // Simulation-time API
    // ------------------------

    // Current simulation time (seconds).
    SimTime now() const {
        return sim_time_seconds_;
    }

    // Total elapsed simulation time since reset (seconds).
    // (If you reset to 0.0, this is identical to now().)
    SimTime total_elapsed_sim_time() const {
        return sim_time_seconds_;
    }

    // Advance simulation time by dt (seconds). No effect while paused.
    void advance(SimDt dt_seconds) {
        if (is_sim_time_paused_) {
            return;
        }
        sim_time_seconds_ += dt_seconds;
    }

    void pause_sim_time() {
        is_sim_time_paused_ = true;
    }

    void resume_sim_time() {
        is_sim_time_paused_ = false;
        // Reset wall tick baseline so "elapsed wall since last tick" doesn't include paused duration.
        last_wall_tick_time_ = WallClock::now();
    }

    bool is_paused() const {
        return is_sim_time_paused_;
    }

    // ------------------------
    // Wall-clock API (optional)
    // ------------------------

    // Wall clock timepoint when reset() was last called.
    WallTimePoint get_wall_start_time() const {
        return wall_clock_start_time_;
    }

    // Elapsed *wall* time since reset (milliseconds).
    std::chrono::duration<double, std::milli> get_total_elapsed_wall_time_ms() const {
        return std::chrono::duration<double, std::milli>(WallClock::now() - wall_clock_start_time_);
    }

    // Reset the wall tick baseline (useful for periodic pacing checks).
    void reset_elapsed_wall_time() {
        last_wall_tick_time_ = WallClock::now();
    }

    // Elapsed *wall* time since last wall tick reset (milliseconds).
    std::chrono::duration<double, std::milli> get_elapsed_wall_time_ms() const {
        return std::chrono::duration<double, std::milli>(WallClock::now() - last_wall_tick_time_);
    }

private:
    // Wall Clock time variables
    WallTimePoint wall_clock_start_time_{};
    WallTimePoint last_wall_tick_time_{};

    // Simulation time variables
    SimTime sim_time_seconds_{0.0};
    bool is_sim_time_paused_{false};
};
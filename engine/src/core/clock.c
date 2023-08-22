//
// Created by jwraynor on 8/22/2023.
//
#include "core/clock.h"
#include "platform/platform.h"

/**
 * Updates the clock. Should be called just before checking the elapsed time.
 * @param clock The clock to update
 */
void clock_update(clock *clock) {
    if (clock->start_time != 0) {
        clock->elapsed_time = platform_get_time() - clock->start_time;
    }
}

/**
 * Starts the clock. Should be called just before the main loop.
 * @param clock The clock to start
 */

void clock_start(clock *clock) {
    clock->start_time = platform_get_time();
    clock->elapsed_time = 0;
}

/**
 * Stops the clock. Should be called just after the main loop.
 * @param clock The clock to stop
 */

void clock_stop(clock *clock) {
    clock->start_time = 0;
}
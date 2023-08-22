//
// Created by jwraynor on 8/22/2023.
//

#pragma once

#include "defines.h"

typedef struct clock {
    f64 start_time;
    f64 elapsed_time;
} clock;

/**
 * Updates the clock. Should be called just before checking the elapsed time.
 * @param clock The clock to update
 */
void clock_update(clock *clock);

/**
 * Starts the clock. Should be called just before the main loop.
 * @param clock The clock to start
 */
void clock_start(clock *clock);

/**
 * Stops the clock. Should be called just after the main loop.
 * @param clock The clock to stop
 */
void clock_stop(clock *clock);
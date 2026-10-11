#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include "memory.h"

#define MAX_REPLACEMENT_FRAMES 64

typedef enum {
    REPLACEMENT_FIFO,
    REPLACEMENT_LRU,
    REPLACEMENT_OPTIMAL,
    REPLACEMENT_CLOCK
} ReplacementAlgorithm;

typedef struct ReplacementState {
    ReplacementAlgorithm algorithm;

    /*
     * FIFO:
     * Points to the next frame to replace.
     */
    int next_victim;

    /*
     * LRU:
     * Stores the last access time of each frame.
     */
    int last_used[MAX_REPLACEMENT_FRAMES];

    /*
     * Global logical clock for LRU.
     */
    int time;

    /*
     * Optimal:
     * Reference string information.
     */
    const int *reference_string;
    int reference_count;
    int current_reference;

    /*
     * Clock / Second Chance:
     * Reference bit for each frame.
     *
     * 1 = recently referenced
     * 0 = not recently referenced
     */
    int reference_bit[MAX_REPLACEMENT_FRAMES];

    /*
     * Clock hand:
     * Points to the next frame to inspect.
     */
    int clock_hand;

} ReplacementState;

/*
 * Initialize replacement algorithm state.
 */
void replacement_init(
    ReplacementState *state,
    ReplacementAlgorithm algorithm
);

/*
 * Select a victim frame for replacement.
 */
int replacement_select_victim(
    ReplacementState *state,
    const Memory *memory
);

/*
 * Record a memory access.
 *
 * Used by LRU and Clock.
 */
void replacement_record_access(
    ReplacementState *state,
    int frame_number
);

/*
 * Return the algorithm name.
 */
const char *replacement_name(
    ReplacementAlgorithm algorithm
);

#endif

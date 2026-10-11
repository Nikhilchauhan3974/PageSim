#include <stddef.h>

#include "replacement.h"

void replacement_init(
    ReplacementState *state,
    ReplacementAlgorithm algorithm
)
{
    if (state == NULL) {
        return;
    }

    state->algorithm = algorithm;

    /*
     * FIFO state.
     */
    state->next_victim = 0;

    /*
     * LRU state.
     */
    state->time = 0;

    for (int i = 0;
         i < MAX_REPLACEMENT_FRAMES;
         i++) {

        state->last_used[i] = 0;
    }

    /*
     * Optimal state.
     */
    state->reference_string = NULL;
    state->reference_count = 0;
    state->current_reference = 0;

    /*
     * Clock / Second Chance state.
     */
    state->clock_hand = 0;

    for (int i = 0;
         i < MAX_REPLACEMENT_FRAMES;
         i++) {

        state->reference_bit[i] = 0;
    }
}

void replacement_record_access(
    ReplacementState *state,
    int frame_number
)
{
    if (state == NULL) {
        return;
    }

    if (frame_number < 0 ||
        frame_number >= MAX_REPLACEMENT_FRAMES) {

        return;
    }

    /*
     * ==========================================
     * LRU
     * ==========================================
     *
     * Increase logical time and record when
     * this frame was last accessed.
     */
    state->time++;

    state->last_used[frame_number] =
        state->time;

    /*
     * ==========================================
     * CLOCK / SECOND CHANCE
     * ==========================================
     *
     * Set the reference bit whenever a page
     * is accessed.
     */
    state->reference_bit[frame_number] = 1;
}

int replacement_select_victim(
    ReplacementState *state,
    const Memory *memory
)
{
    if (state == NULL ||
        memory == NULL ||
        memory->frame_count <= 0) {

        return -1;
    }

    /*
     * ==========================================
     * FIFO
     * ==========================================
     */
    if (state->algorithm == REPLACEMENT_FIFO) {

        int victim = state->next_victim;

        state->next_victim =
            (state->next_victim + 1)
            % memory->frame_count;

        return victim;
    }

    /*
     * ==========================================
     * LRU
     * ==========================================
     */
    if (state->algorithm == REPLACEMENT_LRU) {

        int victim = -1;
        int oldest_time = 0;

        for (int i = 0;
             i < memory->frame_count;
             i++) {

            if (!memory->frames[i].occupied) {
                continue;
            }

            if (victim == -1 ||
                state->last_used[i] < oldest_time) {

                victim = i;

                oldest_time =
                    state->last_used[i];
            }
        }

        return victim;
    }

    /*
     * ==========================================
     * OPTIMAL
     * ==========================================
     *
     * Replace the page whose next use is
     * farthest in the future.
     */
    if (state->algorithm == REPLACEMENT_OPTIMAL) {

        if (state->reference_string == NULL ||
            state->reference_count <= 0) {

            return -1;
        }

        int victim = -1;
        int farthest_use = -1;

        for (int frame = 0;
             frame < memory->frame_count;
             frame++) {

            if (!memory->frames[frame].occupied) {
                continue;
            }

            int page =
                memory->frames[frame].page_number;

            int next_use = -1;

            /*
             * Search for the next occurrence
             * of this page.
             */
            for (int i = state->current_reference + 1;
                 i < state->reference_count;
                 i++) {

                if (state->reference_string[i] == page) {

                    next_use = i;
                    break;
                }
            }

            /*
             * Page is never used again.
             * Replace it immediately.
             */
            if (next_use == -1) {
                return frame;
            }

            /*
             * Otherwise select the page whose
             * next use is farthest away.
             */
            if (next_use > farthest_use) {

                farthest_use = next_use;
                victim = frame;
            }
        }

        return victim;
    }

    /*
     * ==========================================
     * CLOCK / SECOND CHANCE
     * ==========================================
     *
     * The Clock algorithm uses a circular
     * pointer called the clock hand.
     *
     * If reference bit = 0:
     *     Replace this frame.
     *
     * If reference bit = 1:
     *     Give the page a second chance.
     *     Clear the bit and move forward.
     */
    if (state->algorithm == REPLACEMENT_CLOCK) {

        int checked = 0;

        while (checked < memory->frame_count) {

            int frame = state->clock_hand;

            /*
             * Move clock hand forward for the
             * next inspection.
             */
            state->clock_hand =
                (state->clock_hand + 1)
                % memory->frame_count;

            /*
             * Ignore empty frames.
             */
            if (!memory->frames[frame].occupied) {
                checked++;
                continue;
            }

            /*
             * Reference bit = 0:
             * This page has not been recently
             * referenced, so replace it.
             */
            if (state->reference_bit[frame] == 0) {

                return frame;
            }

            /*
             * Reference bit = 1:
             * Give the page a second chance.
             */
            state->reference_bit[frame] = 0;

            checked++;
        }

        /*
         * If every page received a second chance,
         * inspect again for a victim.
         */
        for (int i = 0;
             i < memory->frame_count;
             i++) {

            int frame =
                (state->clock_hand + i)
                % memory->frame_count;

            if (memory->frames[frame].occupied &&
                state->reference_bit[frame] == 0) {

                state->clock_hand =
                    (frame + 1)
                    % memory->frame_count;

                return frame;
            }
        }

        return -1;
    }

    return -1;
}

const char *replacement_name(
    ReplacementAlgorithm algorithm
)
{
    if (algorithm == REPLACEMENT_FIFO) {
        return "FIFO";
    }

    if (algorithm == REPLACEMENT_LRU) {
        return "LRU";
    }

    if (algorithm == REPLACEMENT_OPTIMAL) {
        return "OPTIMAL";
    }

    if (algorithm == REPLACEMENT_CLOCK) {
        return "CLOCK / SECOND CHANCE";
    }

    return "UNKNOWN";
}

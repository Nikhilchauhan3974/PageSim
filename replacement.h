#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include "memory.h"

typedef enum {
    REPLACEMENT_FIFO
} ReplacementAlgorithm;

typedef struct ReplacementState {
    ReplacementAlgorithm algorithm;
    int next_victim;
} ReplacementState;

void replacement_init(
    ReplacementState *state,
    ReplacementAlgorithm algorithm
);

int replacement_select_victim(
    ReplacementState *state,
    const Memory *memory
);

const char *replacement_name(
    ReplacementAlgorithm algorithm
);

#endif

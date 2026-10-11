#ifndef MEMORY_H
#define MEMORY_H

#define MAX_FRAMES 64

/*
 * Forward declarations.
 */
typedef struct ReplacementState ReplacementState;
typedef struct TLB TLB;

typedef struct {
    int frame_number;
    int page_number;
    int occupied;
} Frame;

typedef struct {
    int page_number;
    int frame_number;
    int present;
} PageTableEntry;

typedef struct {
    Frame frames[MAX_FRAMES];
    int frame_count;

    PageTableEntry *page_table;
    int page_count;

    int page_hits;
    int page_faults;

    /*
     * Translation Lookaside Buffer.
     *
     * The TLB caches recent virtual-page
     * to physical-frame translations.
     */
    TLB *tlb;

} Memory;

void memory_init(
    Memory *memory,
    int frame_count,
    int page_count
);

void memory_destroy(
    Memory *memory
);

int memory_access(
    Memory *memory,
    int page_number,
    ReplacementState *replacement
);

void memory_print(
    const Memory *memory
);

void memory_print_stats(
    const Memory *memory
);

#endif

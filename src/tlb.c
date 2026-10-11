#include <stdio.h>

#include "tlb.h"

void tlb_init(
    TLB *tlb
)
{
    if (tlb == NULL) {
        return;
    }

    tlb->entry_count = MAX_TLB_ENTRIES;

    tlb->accesses = 0;
    tlb->hits = 0;
    tlb->misses = 0;

    tlb->next_victim = 0;

    /*
     * Mark every TLB entry as invalid.
     */
    for (int i = 0;
         i < MAX_TLB_ENTRIES;
         i++) {

        tlb->entries[i].page_number = -1;
        tlb->entries[i].frame_number = -1;
        tlb->entries[i].valid = 0;
    }
}

int tlb_lookup(
    TLB *tlb,
    int page_number
)
{
    if (tlb == NULL) {
        return -1;
    }

    tlb->accesses++;

    /*
     * Search every TLB entry.
     */
    for (int i = 0;
         i < tlb->entry_count;
         i++) {

        /*
         * Ignore invalid entries.
         */
        if (!tlb->entries[i].valid) {
            continue;
        }

        /*
         * Check for virtual-page match.
         */
        if (tlb->entries[i].page_number == page_number) {

            tlb->hits++;

            return tlb->entries[i].frame_number;
        }
    }

    /*
     * No matching entry found.
     */
    tlb->misses++;

    return -1;
}

void tlb_insert(
    TLB *tlb,
    int page_number,
    int frame_number
)
{
    if (tlb == NULL) {
        return;
    }

    /*
     * First check whether this page already
     * exists in the TLB.
     *
     * If it does, update its frame mapping.
     */
    for (int i = 0;
         i < tlb->entry_count;
         i++) {

        if (tlb->entries[i].valid &&
            tlb->entries[i].page_number == page_number) {

            tlb->entries[i].frame_number =
                frame_number;

            return;
        }
    }

    /*
     * Look for an empty TLB entry first.
     */
    for (int i = 0;
         i < tlb->entry_count;
         i++) {

        if (!tlb->entries[i].valid) {

            tlb->entries[i].page_number =
                page_number;

            tlb->entries[i].frame_number =
                frame_number;

            tlb->entries[i].valid = 1;

            return;
        }
    }

    /*
     * TLB is full.
     *
     * Replace the entry pointed to by
     * the FIFO victim pointer.
     */
    int victim = tlb->next_victim;

    tlb->entries[victim].page_number =
        page_number;

    tlb->entries[victim].frame_number =
        frame_number;

    tlb->entries[victim].valid = 1;

    /*
     * Move FIFO pointer forward.
     */
    tlb->next_victim =
        (tlb->next_victim + 1)
        % tlb->entry_count;
}

void tlb_invalidate(
    TLB *tlb,
    int page_number
)
{
    if (tlb == NULL) {
        return;
    }

    /*
     * Search for the page and invalidate it.
     */
    for (int i = 0;
         i < tlb->entry_count;
         i++) {

        if (tlb->entries[i].valid &&
            tlb->entries[i].page_number == page_number) {

            tlb->entries[i].valid = 0;

            tlb->entries[i].page_number = -1;

            tlb->entries[i].frame_number = -1;

            return;
        }
    }
}

void tlb_print(
    const TLB *tlb
)
{
    if (tlb == NULL) {
        return;
    }

    printf("\nTranslation Lookaside Buffer\n");
    printf("=============================\n");

    for (int i = 0;
         i < tlb->entry_count;
         i++) {

        if (tlb->entries[i].valid) {

            printf(
                "Entry %d -> Page %d -> Frame %d\n",
                i,
                tlb->entries[i].page_number,
                tlb->entries[i].frame_number
            );

        } else {

            printf(
                "Entry %d -> INVALID\n",
                i
            );
        }
    }

    printf("=============================\n");
}

void tlb_print_stats(
    const TLB *tlb
)
{
    if (tlb == NULL) {
        return;
    }

    double hit_ratio = 0.0;
    double miss_ratio = 0.0;

    if (tlb->accesses > 0) {

        hit_ratio =
            (double)tlb->hits
            / tlb->accesses * 100.0;

        miss_ratio =
            (double)tlb->misses
            / tlb->accesses * 100.0;
    }

    printf("\nTLB Statistics\n");
    printf("==============\n");

    printf(
        "TLB accesses : %d\n",
        tlb->accesses
    );

    printf(
        "TLB hits     : %d\n",
        tlb->hits
    );

    printf(
        "TLB misses   : %d\n",
        tlb->misses
    );

    printf(
        "TLB hit ratio: %.2f%%\n",
        hit_ratio
    );

    printf(
        "TLB miss rate: %.2f%%\n",
        miss_ratio
    );
}

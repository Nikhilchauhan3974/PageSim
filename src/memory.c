#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "replacement.h"
#include "tlb.h"

void memory_init(
    Memory *memory,
    int frame_count,
    int page_count
)
{
    if (memory == NULL) {
        return;
    }

    if (frame_count <= 0 ||
        frame_count > MAX_FRAMES) {

        fprintf(stderr, "Invalid frame count.\n");
        exit(EXIT_FAILURE);
    }

    if (page_count <= 0) {
        fprintf(stderr, "Invalid page count.\n");
        exit(EXIT_FAILURE);
    }

    memory->frame_count = frame_count;
    memory->page_count = page_count;

    memory->page_hits = 0;
    memory->page_faults = 0;

    /*
     * Allocate page table.
     */
    memory->page_table =
        calloc(
            (size_t)page_count,
            sizeof(PageTableEntry)
        );

    if (memory->page_table == NULL) {
        perror("Failed to allocate page table");
        exit(EXIT_FAILURE);
    }

    /*
     * Initialize physical frames.
     */
    for (int i = 0;
         i < frame_count;
         i++) {

        memory->frames[i].frame_number = i;
        memory->frames[i].page_number = -1;
        memory->frames[i].occupied = 0;
    }

    /*
     * Initialize page table.
     */
    for (int i = 0;
         i < page_count;
         i++) {

        memory->page_table[i].page_number = i;
        memory->page_table[i].frame_number = -1;
        memory->page_table[i].present = 0;
    }

    /*
     * Allocate and initialize TLB.
     */
    memory->tlb =
        malloc(sizeof(TLB));

    if (memory->tlb == NULL) {
        perror("Failed to allocate TLB");

        free(memory->page_table);
        memory->page_table = NULL;

        exit(EXIT_FAILURE);
    }

    tlb_init(memory->tlb);
}

void memory_destroy(
    Memory *memory
)
{
    if (memory == NULL) {
        return;
    }

    /*
     * Free page table.
     */
    free(memory->page_table);
    memory->page_table = NULL;

    /*
     * Free TLB.
     */
    free(memory->tlb);
    memory->tlb = NULL;
}

int memory_access(
    Memory *memory,
    int page_number,
    ReplacementState *replacement
)
{
    if (memory == NULL ||
        page_number < 0 ||
        page_number >= memory->page_count) {

        return -1;
    }

    /*
     * ==========================================
     * STEP 1: TLB LOOKUP
     * ==========================================
     */
    int tlb_frame =
        tlb_lookup(
            memory->tlb,
            page_number
        );

    /*
     * TLB HIT
     */
    if (tlb_frame >= 0) {

        /*
         * Verify that the frame still contains
         * the requested page.
         *
         * This protects against stale TLB entries.
         */
        if (tlb_frame < memory->frame_count &&
            memory->frames[tlb_frame].occupied &&
            memory->frames[tlb_frame].page_number ==
                page_number) {

            memory->page_hits++;

            replacement_record_access(
                replacement,
                tlb_frame
            );

            return 0;
        }

        /*
         * Stale TLB entry.
         */
        tlb_invalidate(
            memory->tlb,
            page_number
        );
    }

    /*
     * ==========================================
     * STEP 2: PAGE TABLE LOOKUP
     * ==========================================
     */
    PageTableEntry *entry =
        &memory->page_table[page_number];

    /*
     * PAGE TABLE HIT
     *
     * The page is already in physical memory,
     * but it was not found in the TLB.
     */
    if (entry->present) {

        memory->page_hits++;

        /*
         * Add the translation to the TLB.
         */
        tlb_insert(
            memory->tlb,
            page_number,
            entry->frame_number
        );

        replacement_record_access(
            replacement,
            entry->frame_number
        );

        return 0;
    }

    /*
     * ==========================================
     * STEP 3: PAGE FAULT
     * ==========================================
     */
    memory->page_faults++;

    /*
     * First try to find a free frame.
     */
    for (int i = 0;
         i < memory->frame_count;
         i++) {

        if (!memory->frames[i].occupied) {

            memory->frames[i].occupied = 1;

            memory->frames[i].page_number =
                page_number;

            entry->frame_number = i;
            entry->present = 1;

            /*
             * Insert new translation into TLB.
             */
            tlb_insert(
                memory->tlb,
                page_number,
                i
            );

            replacement_record_access(
                replacement,
                i
            );

            return 1;
        }
    }

    /*
     * ==========================================
     * STEP 4: PAGE REPLACEMENT
     * ==========================================
     */
    if (replacement == NULL) {
        return -2;
    }

    int victim_frame =
        replacement_select_victim(
            replacement,
            memory
        );

    if (victim_frame < 0 ||
        victim_frame >= memory->frame_count) {

        return -2;
    }

    /*
     * Find the page currently occupying
     * the victim frame.
     */
    int old_page =
        memory->frames[victim_frame].page_number;

    /*
     * Remove old page from page table.
     */
    if (old_page >= 0 &&
        old_page < memory->page_count) {

        memory->page_table[old_page].present = 0;

        memory->page_table[old_page].frame_number = -1;

        /*
         * IMPORTANT:
         *
         * The old page's TLB translation is
         * no longer valid because the frame
         * is being reused.
         */
        tlb_invalidate(
            memory->tlb,
            old_page
        );
    }

    /*
     * Load new page into victim frame.
     */
    memory->frames[victim_frame].occupied = 1;

    memory->frames[victim_frame].page_number =
        page_number;

    /*
     * Update page table.
     */
    entry->frame_number = victim_frame;
    entry->present = 1;

    /*
     * Insert new translation into TLB.
     */
    tlb_insert(
        memory->tlb,
        page_number,
        victim_frame
    );

    /*
     * Record access for replacement algorithm.
     */
    replacement_record_access(
        replacement,
        victim_frame
    );

    return 2;
}

void memory_print(
    const Memory *memory
)
{
    if (memory == NULL) {
        return;
    }

    printf("\nPhysical Memory\n");
    printf("-----------------------------\n");

    for (int i = 0;
         i < memory->frame_count;
         i++) {

        if (memory->frames[i].occupied) {

            printf(
                "Frame %d -> Page %d\n",
                i,
                memory->frames[i].page_number
            );

        } else {

            printf(
                "Frame %d -> EMPTY\n",
                i
            );
        }
    }

    printf("-----------------------------\n");

    /*
     * Display TLB contents.
     */
    if (memory->tlb != NULL) {
        tlb_print(memory->tlb);
    }
}

void memory_print_stats(
    const Memory *memory
)
{
    if (memory == NULL) {
        return;
    }

    int total =
        memory->page_hits +
        memory->page_faults;

    double hit_ratio = 0.0;
    double fault_rate = 0.0;

    if (total > 0) {

        hit_ratio =
            (double)memory->page_hits
            / total * 100.0;

        fault_rate =
            (double)memory->page_faults
            / total * 100.0;
    }

    printf("\nSimulation Statistics\n");
    printf("=====================\n");

    printf(
        "Total references : %d\n",
        total
    );

    printf(
        "Page hits        : %d\n",
        memory->page_hits
    );

    printf(
        "Page faults      : %d\n",
        memory->page_faults
    );

    printf(
        "Hit ratio        : %.2f%%\n",
        hit_ratio
    );

    printf(
        "Fault rate       : %.2f%%\n",
        fault_rate
    );

    /*
     * Display TLB statistics.
     */
    if (memory->tlb != NULL) {
        tlb_print_stats(memory->tlb);
    }
}

#include "page_table_demo.h"
#include "memory.h"
#include "replacement.h"

#include <stdio.h>

static void print_page_table(const Memory *memory)
{
    printf("\nPage Table\n");
    printf("----------------------------------------\n");
    printf("%-12s %-15s %-10s\n",
           "Virtual Page", "Physical Frame", "Present");
    printf("----------------------------------------\n");

    for (int i = 0; i < memory->page_count; i++) {
        const PageTableEntry *entry = &memory->page_table[i];

        printf("%-12d ", entry->page_number);

        if (entry->present) {
            printf("%-15d %-10s\n",
                   entry->frame_number, "Yes");
        } else {
            printf("%-15s %-10s\n", "-", "No");
        }
    }

    printf("----------------------------------------\n");
}

void run_page_table_demo(void)
{
    const int frame_count = 3;
    const int page_count = 8;

    const int references[] = {1, 2, 3, 4, 1, 5};

    const int reference_count =
        (int)(sizeof(references) / sizeof(references[0]));

    Memory memory;
    ReplacementState replacement;

    printf("\n========================================\n");
    printf("       PAGE-TABLE INSPECTION DEMO\n");
    printf("========================================\n");

    memory_init(&memory, frame_count, page_count);

    replacement_init(&replacement, REPLACEMENT_FIFO);
    replacement.reference_string = references;
    replacement.reference_count = reference_count;

    printf("Algorithm: FIFO\n");
    printf("Physical frames: %d\n", frame_count);

    for (int i = 0; i < reference_count; i++) {
        replacement.current_reference = i;

        printf("\nAccess %d: Virtual page %d\n",
               i + 1, references[i]);

        int result = memory_access(
            &memory,
            references[i],
            &replacement
        );

        if (result == 0) {
            printf("Result: PAGE HIT\n");
        } else if (result == 1) {
            printf("Result: PAGE FAULT - free frame used\n");
        } else if (result == 2) {
            printf("Result: PAGE FAULT - FIFO replacement\n");
        } else {
            printf("Memory access failed (code %d).\n", result);
            memory_destroy(&memory);
            return;
        }

        print_page_table(&memory);
        memory_print(&memory);
    }

    printf("\nFinal Statistics\n");
    memory_print_stats(&memory);

    memory_destroy(&memory);
}

#include "page_fault_analysis.h"
#include "memory.h"
#include "replacement.h"

#include <stdio.h>

void run_page_fault_analysis(void)
{
    const int frame_count = 3;
    const int page_count = 8;

    const int references[] = {
        1, 2, 3, 1, 2, 4, 1,
        2, 5, 1, 2, 3, 4, 5
    };

    const int reference_count =
        (int)(sizeof(references) / sizeof(references[0]));

    const ReplacementAlgorithm algorithms[] = {
        REPLACEMENT_FIFO,
        REPLACEMENT_LRU,
        REPLACEMENT_OPTIMAL,
        REPLACEMENT_CLOCK
    };

    const int algorithm_count =
        (int)(sizeof(algorithms) / sizeof(algorithms[0]));

    printf("\n============================================\n");
    printf("       PAGE-FAULT ANALYSIS DEMO\n");
    printf("============================================\n");

    printf("Physical frames: %d\n", frame_count);
    printf("Reference count: %d\n", reference_count);

    printf("\nReference string:\n");
    for (int i = 0; i < reference_count; i++) {
        printf("%d", references[i]);

        if (i < reference_count - 1) {
            printf(" -> ");
        }
    }
    printf("\n");

    printf("\n%-12s %10s %12s %12s\n",
           "Algorithm", "Hits", "Faults", "Total");
    printf("------------------------------------------------\n");

    for (int a = 0; a < algorithm_count; a++) {
        Memory memory;
        ReplacementState replacement;

        memory_init(&memory, frame_count, page_count);

        replacement_init(&replacement, algorithms[a]);

        replacement.reference_string = references;
        replacement.reference_count = reference_count;

        for (int i = 0; i < reference_count; i++) {
            replacement.current_reference = i;

            int result = memory_access(
                &memory,
                references[i],
                &replacement
            );

            if (result < 0) {
                printf("Error while simulating %s.\n",
                       replacement_name(algorithms[a]));
                memory_destroy(&memory);
                return;
            }
        }

        printf("%-12s %10d %12d %12d\n",
               replacement_name(algorithms[a]),
               memory.page_hits,
               memory.page_faults,
               memory.page_hits + memory.page_faults);

        memory_destroy(&memory);
    }

    printf("\nAnalysis complete.\n");
}

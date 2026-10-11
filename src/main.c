#include <stdio.h>

#include "memory.h"
#include "replacement.h"

int main(void)
{
    printf("============================================\n");
    printf("              PAGESIM v0.5                  \n");
    printf("============================================\n");
    printf("Demand Paging & Page Replacement Simulator\n\n");

    /*
     * Simulation configuration.
     */
    int frame_count = 3;
    int page_count = 8;

    /*
     * Page reference string.
     */
    int references[] = {
        1, 2, 3, 1, 2, 4, 1, 2, 5, 1, 2, 3, 4, 5
    };

    int reference_count =
        sizeof(references) /
        sizeof(references[0]);

    /*
     * Select replacement algorithm.
     */
    int choice;

    printf("Select Page Replacement Algorithm\n");
    printf("=================================\n");
    printf("1. FIFO\n");
    printf("2. LRU\n");
    printf("3. Optimal\n");
    printf("4. Clock / Second Chance\n");
    printf("\nEnter choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    ReplacementAlgorithm algorithm;

    if (choice == 1) {

        algorithm = REPLACEMENT_FIFO;

    } else if (choice == 2) {

        algorithm = REPLACEMENT_LRU;

    } else if (choice == 3) {

        algorithm = REPLACEMENT_OPTIMAL;

    } else if (choice == 4) {

        algorithm = REPLACEMENT_CLOCK;

    } else {

        printf("Invalid algorithm choice.\n");
        return 1;
    }

    /*
     * Memory and replacement state.
     */
    Memory memory;
    ReplacementState replacement;

    /*
     * Initialize memory.
     */
    memory_init(
        &memory,
        frame_count,
        page_count
    );

    /*
     * Initialize replacement algorithm.
     */
    replacement_init(
        &replacement,
        algorithm
    );

    /*
     * Give Optimal access to the complete
     * reference string.
     */
    replacement.reference_string =
        references;

    replacement.reference_count =
        reference_count;

    replacement.current_reference = 0;

    /*
     * Display simulation configuration.
     */
    printf("\nSimulation Configuration\n");
    printf("========================\n");

    printf(
        "Physical frames       : %d\n",
        frame_count
    );

    printf(
        "Virtual pages         : %d\n",
        page_count
    );

    printf(
        "Replacement algorithm : %s\n",
        replacement_name(
            replacement.algorithm
        )
    );

    printf(
        "Reference count       : %d\n",
        reference_count
    );

    /*
     * Display reference string.
     */
    printf("\nReference String\n");
    printf("================\n");

    for (int i = 0;
         i < reference_count;
         i++) {

        printf("%d", references[i]);

        if (i < reference_count - 1) {
            printf(" -> ");
        }
    }

    printf("\n");

    /*
     * Start simulation.
     */
    printf("\nPage Reference Simulation\n");
    printf("=========================\n");

    /*
     * Process every page reference.
     */
    for (int i = 0;
         i < reference_count;
         i++) {

        int page = references[i];

        /*
         * Tell Optimal which reference
         * is currently being processed.
         */
        replacement.current_reference = i;

        printf(
            "\nReference %d: Page %d\n",
            i + 1,
            page
        );

        /*
         * Access the requested page.
         */
        int result =
            memory_access(
                &memory,
                page,
                &replacement
            );

        /*
         * Interpret result.
         */
        if (result == 0) {

            printf("Result: PAGE HIT\n");

        } else if (result == 1) {

            printf(
                "Result: PAGE FAULT - "
                "loaded into free frame\n"
            );

        } else if (result == 2) {

            printf(
                "Result: PAGE FAULT - "
                "%s replacement performed\n",
                replacement_name(
                    replacement.algorithm
                )
            );

        } else if (result == -2) {

            printf(
                "Result: PAGE FAULT - "
                "replacement failed\n"
            );

        } else {

            printf(
                "Result: INVALID PAGE\n"
            );
        }

        /*
         * Display current physical memory.
         */
        memory_print(&memory);
    }

    /*
     * Display final statistics.
     */
    memory_print_stats(&memory);

    /*
     * Release dynamically allocated memory.
     */
    memory_destroy(&memory);

    printf("\nSimulation completed successfully.\n");

    return 0;
}

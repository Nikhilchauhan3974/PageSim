
#include "address_translation.h"

#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "replacement.h"

#define PAGE_SIZE 1024
#define VIRTUAL_PAGES 8
#define PHYSICAL_FRAMES 4

void run_address_translation_demo(void)
{
    int input_address;
    unsigned int virtual_address;

    Memory memory;
    ReplacementState replacement;

    printf("\n====================================\n");
    printf(" VIRTUAL ADDRESS TRANSLATION DEMO\n");
    printf("====================================\n");

    printf("Page size: %d bytes\n", PAGE_SIZE);
    printf("Virtual pages: %d\n", VIRTUAL_PAGES);
    printf("Physical frames: %d\n", PHYSICAL_FRAMES);

    memory_init(&memory, PHYSICAL_FRAMES, VIRTUAL_PAGES);
    replacement_init(&replacement, REPLACEMENT_LRU);

    printf("\nEnter virtual addresses one by one.\n");
    printf("Enter -1 to finish.\n");

    while (1) {
        printf("\nVirtual address (-1 to finish): ");
        fflush(stdout);

        int input_result = scanf("%d", &input_address);

        if (input_result == EOF) {
            break;
        }

        if (input_result != 1) {
            printf("Invalid input. Please enter an integer.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            if (ch == EOF) {
                break;
            }

            continue;
        }

        if (input_address == -1) {
            break;
        }

        if (input_address < 0) {
            printf("Invalid address. Use -1 to finish.\n");
            continue;
        }

        virtual_address = (unsigned int)input_address;

        if (virtual_address >= VIRTUAL_PAGES * PAGE_SIZE) {
            printf("Error: Virtual address is out of range (0-%d).\n",
                   VIRTUAL_PAGES * PAGE_SIZE - 1);
            continue;
        }

        int page_number = (int)(virtual_address / PAGE_SIZE);
        unsigned int offset = virtual_address % PAGE_SIZE;

        printf("\nVirtual Address: %u\n", virtual_address);
        printf("Virtual Page Number: %d\n", page_number);
        printf("Page Offset: %u\n", offset);

        

        int result = memory_access(&memory, page_number, &replacement);

        if (result < 0) {
            printf("Error: Memory access failed.\n");
            continue;
        }

        int frame_number = memory.page_table[page_number].frame_number;

        unsigned int physical_address =
            (unsigned int)frame_number * PAGE_SIZE + offset;

        printf("TLB lookup completed during memory access.\n");

        if (result == 1 || result == 2) {
            printf("Page fault handled: page loaded into physical memory.\n");
        } else {
            printf("Page already present in physical memory.\n");
        }

        printf("Page Table Lookup: Page %d -> Frame %d\n",
               page_number, frame_number);

        printf("Physical Address: %u\n", physical_address);

        printf("\nCurrent Memory State:\n");
        memory_print(&memory);
    }

    printf("\n====================================\n");
    printf(" FINAL MEMORY STATISTICS\n");
    printf("====================================\n");

    memory_print_stats(&memory);

    printf("\nAddress translation demo finished.\n");

    memory_destroy(&memory);
}

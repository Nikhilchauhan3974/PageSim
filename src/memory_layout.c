
#include "memory_layout.h"

#include <stdio.h>
#include <stdlib.h>

int initialized_global = 100;
int uninitialized_global;

static const char message[] = "PageSim Memory Layout Demo";

static void sample_function(void)
{
    int local_variable = 10;

    printf("  Local variable address (stack): %p\n",
           (void *)&local_variable);
}

void run_memory_layout_demo(void)
{
    int stack_variable = 20;

    int *heap_variable = malloc(sizeof(*heap_variable));

    if (heap_variable == NULL) {
        fprintf(stderr, "Error: Heap allocation failed.\n");
        return;
    }

    *heap_variable = 30;

    printf("\n====================================\n");
    printf(" LINUX PROCESS MEMORY LAYOUT DEMO\n");
    printf("====================================\n");

    printf("\n1. TEXT SEGMENT\n");
    printf("  Function address: %p\n",
           (void *)sample_function);

    printf("\n2. DATA SEGMENT\n");
    printf("  Initialized global address: %p\n",
           (void *)&initialized_global);
    printf("  Initialized global value: %d\n",
           initialized_global);

    printf("\n3. BSS SEGMENT\n");
    printf("  Uninitialized global address: %p\n",
           (void *)&uninitialized_global);
    printf("  Initial value: %d\n",
           uninitialized_global);

    printf("\n4. READ-ONLY DATA\n");
    printf("  Message address: %p\n",
           (const void *)message);
    printf("  Message: %s\n", message);

    printf("\n5. HEAP\n");
    printf("  Allocated memory address: %p\n",
           (void *)heap_variable);
    printf("  Stored value: %d\n", *heap_variable);

    printf("\n6. STACK\n");
    printf("  Main function local variable address: %p\n",
           (void *)&stack_variable);
    printf("  Local variable value: %d\n", stack_variable);

    sample_function();

    free(heap_variable);
    heap_variable = NULL;

    printf("\nHeap memory released successfully.\n");
    printf("Memory layout demonstration completed.\n");
}

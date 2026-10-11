
#include "allocation_demo.h"

#include <stdio.h>
#include <stdlib.h>

void run_allocation_demo(void)
{
    printf("\n====================================\n");
    printf(" DYNAMIC MEMORY ALLOCATION DEMO\n");
    printf("====================================\n");

    /* 1. malloc() */
    printf("\n1. malloc()\n");

    int *numbers = malloc(3 * sizeof(*numbers));

    if (numbers == NULL) {
        fprintf(stderr, "malloc failed.\n");
        return;
    }

    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;

    printf("Allocated 3 integers.\n");
    printf("Values: %d %d %d\n",
           numbers[0], numbers[1], numbers[2]);

    /* 2. calloc() */
    printf("\n2. calloc()\n");

    int *zero_values = calloc(3, sizeof(*zero_values));

    if (zero_values == NULL) {
        fprintf(stderr, "calloc failed.\n");
        free(numbers);
        return;
    }

    printf("Initial values: %d %d %d\n",
           zero_values[0], zero_values[1], zero_values[2]);

    /* 3. realloc() */
    printf("\n3. realloc()\n");

    int *resized = realloc(numbers, 5 * sizeof(*numbers));

    if (resized == NULL) {
        fprintf(stderr, "realloc failed.\n");
        free(numbers);
        free(zero_values);
        return;
    }

    numbers = resized;
    numbers[3] = 40;
    numbers[4] = 50;

    printf("Resized allocation to 5 integers.\n");
    printf("Values: ");

    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }

    printf("\n");

    /* 4. free() */
    printf("\n4. free()\n");

    free(numbers);
    free(zero_values);

    numbers = NULL;
    zero_values = NULL;

    printf("Allocated memory released successfully.\n");
    printf("Memory allocation demo completed.\n");
}

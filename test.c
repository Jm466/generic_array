#include "garray.h"
#include <stdio.h>

GARRAY_DECLARE(int)
GARRAY_IMPLEMENT(int)

void print_garray_int(garray_int a)
{
    if (garray_int_size(a) == 0) {
        printf("[][]\n");
        return;
    }

    int defaultv = -1;

    printf("[%i", *garray_int_at_default(a, 0, &defaultv));

    if (garray_int_size(a) == 1) {
        printf("][%i]\n", ___GARRAY_GET_VALUE_SETTED(a->nodes, 0));
        return;
    }

    garray_int_iter it = garray_int_iter_new(a);

    for (garray_int_iter_next(it); garray_int_iter_condition_free(it); garray_int_iter_next(it))
        printf(", %i", *garray_int_iter_get(it));

    printf("][");

    for (garray_index i = 0; i < a->nodes_allocated * ___GARRAY_ELEMENTS_PER_NODE; i++)
        printf("%i", ___GARRAY_GET_VALUE_SETTED(a->nodes, i) ? 1 : 0);

    printf("]\n");
}

int
int_ascending(int const* left, int const* right)
{
    return *right - *left;
}

int
int_descending(int const* left, int const* right)
{
    return *left - *right;
}

bool
even(int const* element, void* data)
{
    return *element % 2 == 0;
}

int
main()
{
    garray_int ai = garray_int_new();

    garray_int_add(ai, 0);
    garray_int_add(ai, 1);
    garray_int_add(ai, 2);
    garray_int_add(ai, 3);
    garray_int_add(ai, 4);
    garray_int_add(ai, 5);
    garray_int_add(ai, 6);
    garray_int_add(ai, 7);
    garray_int_add(ai, 8);
    garray_int_add(ai, 9);
    printf("Array int -> Size:%i Allocated:%i\n", ai->num_elements, ai->nodes_allocated);

    print_garray_int(ai);

    garray_int_set(ai, 1, 12);
    garray_int_set(ai, 2, 13);
    garray_int_set(ai, 3, 14);

    print_garray_int(ai);

    garray_int_remove(ai, 5);
    garray_int_remove(ai, 7);

    printf("removed: ");
    print_garray_int(ai);

    garray_int_sort(ai, int_ascending);
    printf("sorted ascending: ");
    print_garray_int(ai);

    garray_int_sort(ai, int_descending);
    printf("sorted descending: ");
    print_garray_int(ai);

    garray_int_sort(ai, int_ascending);
    printf("sorted ascending: ");
    print_garray_int(ai);

    garray_int_add(ai, 21);
    garray_int_add(ai, 22);
    garray_int_add(ai, 23);

    garray_int int_query = garray_int_query(ai, NULL, even);
    printf("Only even :");
    print_garray_int(int_query);
    garray_int_free(int_query);

    printf("added three elements: ");
    print_garray_int(ai);


    garray_int_set(ai, 1000, 99);

    printf("setted value at 1000: ");
    print_garray_int(ai);

    garray_int_collapse(ai);

    printf("collapse: ");
    print_garray_int(ai);

    garray_int_free(ai);

    garray_int a = garray_int_new();

    garray_int_iter it = garray_int_iter_new(a);
    garray_int_iter_free(it);
    garray_int_free(a);

    return 0;
}

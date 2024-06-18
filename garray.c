#include "garray.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ELEMENTS_PER_NODE 8
#define LOG_B2_ELEMENTS_PER_NODE 3

#define GARRAY_SET_VALUE_SETTED(garray, position)                               \
    (garray)->values_setted[(position) >> LOG_B2_ELEMENTS_PER_NODE] =           \
                                                                                ((garray)->values_setted[(position) >> LOG_B2_ELEMENTS_PER_NODE])   \
                                                                                |  (1u << ((position) % ELEMENTS_PER_NODE))

#define GARRAY_UNSET_VALUE_SETTED(garray, position)                             \
    (garray)->values_setted[(position) >> LOG_B2_ELEMENTS_PER_NODE] =           \
                                                                                ((garray)->values_setted[(position) >> LOG_B2_ELEMENTS_PER_NODE]) & \
                                                                                ~(1u << ((position) % ELEMENTS_PER_NODE))

#define GARRAY_GET_VALUE_SETTED(garray, position)                        \
    (((garray)->values_setted[(position) >> LOG_B2_ELEMENTS_PER_NODE]) & \
     (1u << ((position) % ELEMENTS_PER_NODE)))

static void* TMP_PTR;
#define REALLOC(ptr, new_size, error_message) {\
        TMP_PTR = ptr; \
        if((ptr = realloc(ptr, new_size)) == NULL) { \
            perror(error_message); \
            free(TMP_PTR); \
            abort(); \
        }\
    }

typedef int8_t* array_t;

struct generic_array {
    garray_index bytes_allocated; //Total number of bytes allocated for the array
    garray_index bytes_allocated_values_setted; // Total number of bytes allocated for values_setted
    garray_index num_elements; //Total number of elements inside the array
    garray_index next_free; //The index of the next free element
    garray_index element_size; //The size of each element in bytes
    array_t values_setted; //An array of bytes that stores whether an element is set or not for each element
    array_t array;
};

struct generic_array_iterator {
    bool valid_index;
    garray_index index;
    struct generic_array* garray;
};



garray
___garray_new(garray_index element_size)
{
    garray garray = malloc(sizeof(struct generic_array));

    if (garray == NULL) {
        perror("___garray_new(): malloc\n");
        abort();
    }

    garray->bytes_allocated = 0;
    garray->bytes_allocated_values_setted = 0;
    garray->num_elements = 0;
    garray->next_free = 0;
    garray->element_size = element_size;
    garray->values_setted = NULL;
    garray->array = NULL;

    return garray;
}

garray
___garray_new_preallocated(garray_index num_elements_preallocated,
                           garray_index element_size)
{
    garray a = ___garray_new(element_size);

    a->bytes_allocated = num_elements_preallocated * element_size;
    a->bytes_allocated_values_setted = (num_elements_preallocated + 7) >> LOG_B2_ELEMENTS_PER_NODE;
    a->array = calloc(num_elements_preallocated, element_size);
    a->values_setted = calloc(a->bytes_allocated_values_setted, 1);

    if (a->array == NULL || a->values_setted == NULL) {
        perror("___garray_new_preallocated(): calloc\n");
        abort();
    }

    return a;
}

static bool
check_resizing(garray a)
{
    if (a->next_free * a->element_size < a->bytes_allocated)
        return false;

    garray_index previous_allocation = a->bytes_allocated;

    a->bytes_allocated = previous_allocation == 0 ? a->element_size : previous_allocation << 1;

    if (a->bytes_allocated <= previous_allocation) {
        if (previous_allocation < GARRAY_MAX_VALUE)
            a->bytes_allocated = GARRAY_MAX_VALUE;
        else {
            perror("check_resizing(): posible overflow of the garray_index type, try setting it to a bigger data type\n");
            abort();
        }
    }

    REALLOC(a->array, a->bytes_allocated, "check_resizing(): realloc 1\n");

    memset(a->array + previous_allocation, 0, a->bytes_allocated - previous_allocation);

    // The following expression returns the number of bytes needed to store a->values_setted given
    // the number of bytes needed to store a->array
#define VALUES_SETTED_SIZE(array_size) (((array_size) / a->element_size + 7) >> LOG_B2_ELEMENTS_PER_NODE)

    garray_index previous_allocation_values = VALUES_SETTED_SIZE(previous_allocation);
    a->bytes_allocated_values_setted = VALUES_SETTED_SIZE(a->bytes_allocated);

    if (a->bytes_allocated_values_setted == previous_allocation_values) //If a->values_setted need not allocation finish
        return true;

    REALLOC(a->values_setted, a->bytes_allocated, "check_resizing(): realloc 2\n");

    memset(a->values_setted + previous_allocation_values, 0,
           VALUES_SETTED_SIZE(a->bytes_allocated - previous_allocation));

    return true;
}

#define get_element(a, position)\
    ((a)->array + (position) * (a)->element_size)

static garray_index
get_next_free(garray a)
{
    check_resizing(a);

    if (!GARRAY_GET_VALUE_SETTED(a, a->next_free))
        return a->next_free;

    while (GARRAY_GET_VALUE_SETTED(a, a->next_free)) {
        a->next_free++;
        check_resizing(a);
    }

    return a->next_free;
}

garray_index
___garray_add(garray a, const void* data)
{
    garray_index pos = get_next_free(a);

    memcpy(get_element(a, pos), data, a->element_size);
    GARRAY_SET_VALUE_SETTED(a, pos);
    a->num_elements++;

    return pos;
}

const void*
___garray_at(garray a, garray_index position)
{
    if (position * a->element_size >= a->bytes_allocated) {
        perror("garray_at(): position out of bounds\n");
        abort();
    }

    if (!GARRAY_GET_VALUE_SETTED(a, position)) {
        perror("garray_at(): position not setted\n");
        abort();
    }

    return get_element(a, position);
}

const void*
___garray_at_default(garray a, garray_index position, const void* default_value)
{
    if (position * a->element_size >= a->bytes_allocated)
        return default_value;

    if (!GARRAY_GET_VALUE_SETTED(a, position))
        return default_value;

    return get_element(a, position);
}

void
___garray_set(garray a, garray_index position, const void* restrict data)
{
    if (position * a->element_size >= a->bytes_allocated) {
        garray_index old_next_free = a->next_free;
        a->next_free = position;

        while (check_resizing(a));

        a->next_free = old_next_free;

        a->num_elements++;
    }

    memcpy(get_element(a, position), data, a->element_size);
    GARRAY_SET_VALUE_SETTED(a, position);
}

void
___garray_remove(garray a, garray_index position)
{
    GARRAY_UNSET_VALUE_SETTED(a, position);
    a->num_elements--;

    if (position < a->next_free)
        a->next_free = position;
}

garray_index
___garray_size(garray a)
{
    return a->num_elements;
}

garray
___garray_clone(garray a)
{
    garray new_a = ___garray_new(a->element_size);

    new_a->bytes_allocated = a->bytes_allocated;
    new_a->bytes_allocated_values_setted = a->bytes_allocated_values_setted;
    new_a->num_elements = a->num_elements;
    new_a->next_free = a->next_free;

    new_a->array = malloc(a->bytes_allocated);
    new_a->values_setted = malloc(a->bytes_allocated_values_setted);

    memcpy(new_a->array, a->array, a->bytes_allocated);
    memcpy(new_a->values_setted, a->values_setted, a->bytes_allocated_values_setted);

    return new_a;
}

void
___garray_collapse(garray a)
{
    garray_index tail = (a->bytes_allocated / a->element_size) - 1;

    for (garray_index head = 0; head < tail; head++) {
        if (GARRAY_GET_VALUE_SETTED(a, head))
            continue;

        while (!GARRAY_GET_VALUE_SETTED(a, tail)) {
            if (head == --tail)
                goto collapse_break_loop;
        }

        memcpy(get_element(a, head), get_element(a, tail), a->element_size);

        GARRAY_UNSET_VALUE_SETTED(a, tail);
        GARRAY_SET_VALUE_SETTED(a, head);
    }

collapse_break_loop :;

    /* If the last element is unset, do not count it */
    a->next_free = GARRAY_GET_VALUE_SETTED(a, tail) ? tail + 1 : tail;

    a->bytes_allocated = (a->next_free * a->element_size) + a->element_size;

    if (a->bytes_allocated == 0) {
        free(a->array);
        a->array = NULL;
    } else
        REALLOC(a->array, a->bytes_allocated, "___garray_collapse(): realloc\n");
}

garray
___garray_sort(garray a, int criteria(void const*, void const*))
{
    a = ___garray_clone(a);

    ___garray_collapse(a);

    qsort(a->array, a->num_elements, a->element_size, criteria);

    return a;
}

void
___garray_free(garray a)
{

    if (a->array != NULL) {
        free(a->array);
        free(a->values_setted);
    }

    free(a);
}

garray_iter
___garray_iter_new(garray a)
{

    garray_iter new_iter = malloc(sizeof(struct generic_array_iterator));

    if (new_iter == NULL) {
        perror("___garray_iter_new(): malloc\n");
        abort();
    }

    new_iter->garray = a;
    new_iter->index = 0;
    new_iter->valid_index = a->array == NULL ? false : GARRAY_GET_VALUE_SETTED(a,
                                                                               0);

    return new_iter;
}

bool
___garray_iter_condition(garray_iter iterator)
{
    return iterator->valid_index;
}

#define fast_condition(iter) iter->valid_index

void
___garray_iter_free(garray_iter iterator)
{
    free(iterator);
}

bool
___garray_iter_condition_free(garray_iter iterator)
{
    if (fast_condition(iterator))
        return true;

    ___garray_iter_free(iterator);
    return false;
}

void
___garray_iter_next(garray_iter iterator)
{
    const garray_index max_index = iterator->garray->bytes_allocated /
                                   iterator->garray->element_size;

    while (++iterator->index < max_index) {
        if (GARRAY_GET_VALUE_SETTED(iterator->garray, iterator->index)) {
            iterator->valid_index = true;
            return;
        }
    }

    iterator->valid_index = false;
}

void
___garray_iter_previous(garray_iter iterator)
{

    /* index has an offset of +1,                   */
    /* so that when it reaches 0 that would be -1,  */
    /* because whe are using unsigned types         */
    while (iterator->index-- > 0) {
        if (GARRAY_GET_VALUE_SETTED(iterator->garray, iterator->index)) {
            iterator->valid_index = true;
            return;
        }
    }

    iterator->valid_index = false;
}

void const*
___garray_iter_get(garray_iter iterator)
{
    return get_element(iterator->garray, iterator->index);
}
#define fast_iter_get(iter) get_element(iter->garray, iter->index)

void
___garray_iter_set(garray_iter iterator, void const* data)
{
    ___garray_set(iterator->garray, iterator->index, data);
}

garray_index
___garray_iter_get_index(garray_iter iterator)
{
    return iterator->index;
}

bool
___garray_iter_set_index(garray_iter iterator, garray_index index)
{
    if (index >= iterator->garray->bytes_allocated / iterator->garray->element_size)
        return false;

    iterator->valid_index = GARRAY_GET_VALUE_SETTED(iterator->garray,
                                                    iterator->index);

    iterator->index = index;
    return true;
}

bool
___garray_contains(garray a, const void* value,
                   bool comparator(void const* left, void const* right))
{
    garray_iter iter;

    for (iter = ___garray_iter_new(a); ___garray_iter_condition_free(iter);
         ___garray_iter_next(iter)) {
        if (comparator(&value, fast_iter_get(iter))) {
            ___garray_iter_free(iter);
            return true;
        }
    }

    return false;
}

garray
___garray_query(
    garray a, void* data,
    bool condition(void const* value, void* data))
{
    garray new_a = ___garray_new(a->element_size);
    void const* current = NULL;

    for (garray_iter it = ___garray_iter_new(a); ___garray_iter_condition_free(it);
         ___garray_iter_next(it)) {
        current = ___garray_iter_get(it);

        if (condition(current, data))
            ___garray_add(new_a, current);
    }

    return new_a;
}

void const*
___garray_get(garray a, void* data, bool condition(void const* value,
                                                   void* data))
{
    void const* current = NULL;
    garray_iter it;

    for (it = ___garray_iter_new(a); ___garray_iter_condition(it);
         ___garray_iter_next(it)) {
        current = ___garray_iter_get(it);

        if (condition(current, data)) {
            ___garray_iter_free(it);
            return current;
        }
    }

    ___garray_iter_free(it);
    return NULL;
}

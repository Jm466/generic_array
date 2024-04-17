#ifndef GENERIC_ARRAY
#define GENERIC_ARRAY

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GARRAY_VERSION 1.0

// Type used to index the values of the array
//
// The number of elements in the array can not surpass the maximun value of this
// type.
typedef unsigned int garray_index;
#define GARRAY_MAX_VALUE UINT_MAX

/*
 *  Generic Array library
 *
 *  This library adds the functionaly of generics arrays.
 *  It works by using the GARRAY_DECLARE() and GARRAY_IMPLEMENT() macros.
 *
 *  GARRAY_IMPLEMENT(DATA_TYPE) expands to all the function implementations
 * required for an array of DATA_TYPE GARRAY_DECLARE(DATA_TYPE) expands to a
 * declaration of all funcions required for an array of DATA_TYPE
 *
 *  You would use GARRAY_IMPLEMENT(DATA_TYPE) to implement an array of that type
 * and then if you need to use an array of that type in other module you would
 * use GARRAY_DECLARE(DATA_TYPE)
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! DATA_TYPE MUST BE A SINGLE WORD OR ELSE IT MAY NOT WORK  !
 * !                                                          !
 * ! GARRAY_DECLARE(unsigned int) it is no allowed.           !
 * !                                                          !
 * ! You would need to do something like:                     !
 * !                                                          !
 * ! typedef unsigned int myType;                             !
 * ! GARRAY_DECLARE(myType)                                   !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *  You may use GARRAY_DECLARE(DATA_TYPE) multiple times per type but only once
 * per type for GARRAY_IMPLEMENT(DATA_TYPE)
 *
 *
 *
 *  Now follows the documentation for every function in the library, note that
 * the function name changes based on the type; the change always occurs as a
 * sufix in the name. e.g. garray_new -> garray_int_new, garray_char_new, etc.
 * The types garray and garray_iter change also to garray_int_iter and
 * garray_char_iter respectively. The following documentation assumes that the
 * type is TYPE.
 *
 *
 * Returns an empty new array of type TYPE
 * garray_TYPE garray_TYPE_new();
 *
 * Same as garray_TYPE_new() but the array start with a size of
 * num_elements_preallocated
 * garray_TYPE garray_TYPE_new_prealocated(garray_index
 * num_elements_preallocated)
 *
 * Appends an element to the array
 * garray_index garray_TYPE_add(garray_TYPE a, TYPE data);
 *
 * Returns an unmodifiable pointer to the value at position.
 * Returns NULL if position its outside of bounds or the value is unset.
 * TYPE const *garray_TYPE_at(garray_TYPE a, garray_index position);
 *
 * Returns an unmodifiable pointer to the value at position.
 * Returns default if if position its outside of bounds or the value is unset.
 * TYPE const *garray_TYPE_at_default(garray_TYPE a, garray_index position,
 * TYPE const *default_value);
 *
 * Returns an unmodifiable pointer to the first value that matches the
 * condition, NULL if none matches.
 * @param data This value will be passed to condition each time its called
 * TYPE const *garray_TYPE_get(garray_TYPE a, void* data,
 *                          bool condition(TYPE const *value, void* data))
 *
 * Returns a new garray that contains all the elements that matches condition.
 * garray_TYPE garray_TYPE_query(garray_TYPE a, void* data
 *                          bool condition(TYPE const *value, void* data))
 *
 * Sets the value at position, will increment the allocated space for the array
 * if necessary
 * void garray_TYPE_set(garray_TYPE a, garray_index position, TYPE data);
 *
 * Unsets the value at position. When iterating, unsetted values will
 * be ignored. Also garray_TYPE_at() aborts if the value is unse
 * void garray_TYPE_remove(garray_TYPE a, garray_index position);
 *
 * Returns the number of elements of the array
 * garray_index garray_TYPE_size(garray_TYPE a);
 *
 * Returns an exact copy of the array. Iterators of the original array are not
 * affected
 * garray_TYPE garray_TYPE_clone(garray_TYPE a);
 *
 * It gets rid of the unsetted values in the array reducing the allocated space
 * to the number of elements in the array
 * void garray_TYPE_collapse(garray_TYPE a);
 *
 * Collapses and sorts the array according to criteria
 * `criteria` == 0: None is before the other
 * `criteria` > 0: Left is before right
 * `criteria` < 0: Left is after right
 * void garray_TYPE_sort(garray_TYPE a,
 *                      int criteria(TYPE const *left, TYPE const *right));
 *
 * Returns true if value is contained in the array, according to comparator
 * bool garray_TYPE_contains(garray_TYPE a, TYPE value,
 *                      bool comparator(TYPE const *left, TYPE const *right))
 *
 *
 *
 * Frees the array
 * void garray_TYPE_free(garray_TYPE a);
 *
 * Returns a new iterator for the array
 * Iterators allow you to iterate easily over the array
 * Note that, after creating a new iterator, modifiying the structure of the
 * array by calling garray_TYPE_collapse(), garray_TYPE_sort() or any other
 * function that alters the number of elements or its positions in the
 * array(like garray_TYPE_set() when called with a position that would require
 * the array to expand), can get the iterator into an inconsistent state that
 * could lead to a crash
 * garray_TYPE_iter garray_TYPE_iter_new(garray_TYPE a);
 *
 * Returns false if the iterator is at the end of the array,
 * true in other case
 * bool garray_TYPE_iter_condition(garray_TYPE_iter iterator);
 *
 * Same as garray_TYPE_iter_condition but when it reaches the end
 * automatically frees the iterator bool
 * garray_TYPE_iter_condition_free(garray_TYPE_iter_int iterator);
 *
 * Advances the iterator to the next available value
 * void garray_TYPE_iter_next(garray_TYPE_iter iterator);
 *
 * Recedes the iterator to the previous available value
 * void garray_TYPE_iter_previous(garray_TYPE_iter iterator);
 *
 * Returns a pointer to the value at the current iterator position
 * TYPE *garray_TYPE_iter_get(garray_TYPE_iter iterator);
 *
 * Sets the value at the current iterator position.
 * TYPE *garray_TYPE_iter_set(garray_TYPE_iter iterator, TYPE data);
 *
 * Returns the index of the array in the current iterator position
 * garray_index garray_TYPE_iter_get_index(garray_TYPE_iter iterator);
 *
 * Sets the index of the iterator of the array at the specified position
 * garray_index garray_TYPE_iter_set_index(garray_TYPE_iter iterator,
 *                                          garray_index index);
 *
 * Frees the iterator
 * void garray_TYPE_iter_free(garray_TYPE_iter_int iterator);
 */

// Declare the array of type DATA_TYPE to use it when the array for that type
// has already been implemented at some other place
#define GARRAY_DECLARE(DATA_TYPE)                                              \
  typedef struct ___generic_garray_##DATA_TYPE *garray_##DATA_TYPE;            \
  typedef struct ___generic_garray_##DATA_TYPE##_iterator                      \
      *garray_##DATA_TYPE##_iter;                                              \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_new();                               \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_new_prealocated(                     \
      garray_index num_elements_preallocated);                                 \
                                                                               \
  garray_index garray_##DATA_TYPE##_add(garray_##DATA_TYPE a, DATA_TYPE data); \
                                                                               \
  DATA_TYPE const *garray_##DATA_TYPE##_at(garray_##DATA_TYPE a,               \
                                           garray_index position);             \
                                                                               \
  DATA_TYPE const *garray_##DATA_TYPE##_at_default(                            \
      garray_##DATA_TYPE a, garray_index position,                             \
      DATA_TYPE const *default_value);                                         \
                                                                               \
  DATA_TYPE const *garray_##DATA_TYPE##_get(                                   \
      garray_##DATA_TYPE a, void *data,                                        \
      bool condition(DATA_TYPE const *value, void *data));                     \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_query(                               \
      garray_##DATA_TYPE a, void *data,                                        \
      bool condition(DATA_TYPE const *value, void *data));                     \
                                                                               \
  garray_index garray_##DATA_TYPE##_size(garray_##DATA_TYPE a);                \
                                                                               \
  void garray_##DATA_TYPE##_remove(garray_##DATA_TYPE a,                       \
                                   garray_index position);                     \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_clone(garray_##DATA_TYPE a);         \
                                                                               \
  void garray_##DATA_TYPE##_collapse(garray_##DATA_TYPE a);                    \
                                                                               \
  void garray_##DATA_TYPE##_sort(                                              \
      garray_##DATA_TYPE a,                                                    \
      int criteria(DATA_TYPE const *, DATA_TYPE const *));                     \
                                                                               \
  bool garray_##DATA_TYPE##_contains(                                          \
      garray_##DATA_TYPE a, DATA_TYPE value,                                   \
      bool comparator(DATA_TYPE const *left, DATA_TYPE const *right));         \
                                                                               \
  void garray_##DATA_TYPE##_free(garray_##DATA_TYPE a);                        \
                                                                               \
  garray_##DATA_TYPE##_iter garray_##DATA_TYPE##_iter_new(                     \
      garray_##DATA_TYPE a);                                                   \
                                                                               \
  bool garray_##DATA_TYPE##_iter_condition(                                    \
      garray_##DATA_TYPE##_iter iterator);                                     \
                                                                               \
  bool garray_##DATA_TYPE##_iter_condition_free(                               \
      garray_##DATA_TYPE##_iter iterator);                                     \
                                                                               \
  void garray_##DATA_TYPE##_iter_next(garray_##DATA_TYPE##_iter iterator);     \
                                                                               \
  void garray_##DATA_TYPE##_iter_previous(garray_##DATA_TYPE##_iter iterator); \
                                                                               \
  DATA_TYPE const *garray_##DATA_TYPE##_iter_get(                              \
      garray_##DATA_TYPE##_iter iterator);                                     \
                                                                               \
  void garray_##DATA_TYPE##_iter_set(garray_##DATA_TYPE##_iter iterator,       \
                                     DATA_TYPE data);                          \
                                                                               \
  garray_index garray_##DATA_TYPE##_iter_get_index(                            \
      garray_##DATA_TYPE##_iter iterator);                                     \
                                                                               \
  bool garray_##DATA_TYPE##_iter_set_index(garray_##DATA_TYPE##_iter iterator, \
                                           garray_index index);                \
                                                                               \
  void garray_##DATA_TYPE##_iter_free(garray_##DATA_TYPE##_iter iterator);

#define ___GARRAY_ELEMENTS_PER_NODE 8

// log_2(___GARRAY_ELEMENTS_PER_NODE)
#define ___GARRAY_LOG_B2_ELEMENTS_PER_NODE 3

#define ___GARRAY_SET_VALUE_SETTED(garray, position)                           \
  (garray)[(position) >> ___GARRAY_LOG_B2_ELEMENTS_PER_NODE].values_setted =   \
      ((garray)[(position) >> ___GARRAY_LOG_B2_ELEMENTS_PER_NODE])             \
          .values_setted |                                                     \
      (1u << ((position) % ___GARRAY_ELEMENTS_PER_NODE))

#define ___GARRAY_UNSET_VALUE_SETTED(garray, position)                         \
  (garray)[(position) >> ___GARRAY_LOG_B2_ELEMENTS_PER_NODE].values_setted =   \
      ((garray)[(position) >> ___GARRAY_LOG_B2_ELEMENTS_PER_NODE])             \
          .values_setted &                                                     \
      ~(1u << ((position) % ___GARRAY_ELEMENTS_PER_NODE))

#define ___GARRAY_GET_VALUE_SETTED(garray, position)                           \
  (((garray)[(position) >> ___GARRAY_LOG_B2_ELEMENTS_PER_NODE])                \
       .values_setted &                                                        \
   (1u << ((position) % ___GARRAY_ELEMENTS_PER_NODE)))

// Implement the array for the type DATA_TYPE --------------------------------
#define GARRAY_IMPLEMENT(DATA_TYPE)                                            \
  typedef struct ___generic_garray_##DATA_TYPE *garray_##DATA_TYPE;            \
  typedef struct ___generic_garray_##DATA_TYPE##_iterator                      \
      *garray_##DATA_TYPE##_iter;                                              \
                                                                               \
  struct ___generic_garray_node_##DATA_TYPE {                                  \
    uint8_t values_setted;                                                     \
    DATA_TYPE elements[___GARRAY_ELEMENTS_PER_NODE];                           \
  };                                                                           \
                                                                               \
  struct ___generic_garray_##DATA_TYPE {                                       \
    garray_index nodes_allocated;                                              \
    garray_index num_elements;                                                 \
    garray_index next_free;                                                    \
    struct ___generic_garray_node_##DATA_TYPE *nodes;                          \
  };                                                                           \
                                                                               \
  struct ___generic_garray_##DATA_TYPE##_iterator {                            \
    garray_##DATA_TYPE garray;                                                 \
    bool valid_index;                                                          \
    garray_index index;                                                        \
  };                                                                           \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_new() {                              \
    garray_##DATA_TYPE garray =                                                \
        malloc(sizeof(struct ___generic_garray_##DATA_TYPE));                  \
                                                                               \
    if (garray == NULL) {                                                      \
      perror("garray_" #DATA_TYPE "_new(): malloc\n");                         \
      abort();                                                                 \
    }                                                                          \
                                                                               \
    garray->nodes_allocated = 0;                                               \
    garray->num_elements = 0;                                                  \
    garray->next_free = 0;                                                     \
    garray->nodes = NULL;                                                      \
                                                                               \
    return garray;                                                             \
  }                                                                            \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_new_prealocated(                     \
      garray_index num_elements_prealocated) {                                 \
    garray_##DATA_TYPE a = garray_##DATA_TYPE##_new();                         \
                                                                               \
    a->nodes_allocated =                                                       \
        num_elements_prealocated >> ___GARRAY_LOG_B2_ELEMENTS_PER_NODE;        \
    a->nodes = calloc(a->nodes_allocated,                                      \
                      sizeof(struct ___generic_garray_node_##DATA_TYPE));      \
                                                                               \
    if (a->nodes == NULL) {                                                    \
      perror("garray_" #DATA_TYPE "_new_prealocated(): calloc\n");             \
      abort();                                                                 \
    }                                                                          \
                                                                               \
    return a;                                                                  \
  }                                                                            \
                                                                               \
  static bool ___garray_check_resizing##DATA_TYPE(garray_##DATA_TYPE a) {      \
    if (a->next_free < a->nodes_allocated                                      \
                           << ___GARRAY_LOG_B2_ELEMENTS_PER_NODE)              \
      return false;                                                            \
                                                                               \
    garray_index previous_length = a->nodes_allocated;                         \
                                                                               \
    a->nodes_allocated = previous_length == 0 ? 1 : previous_length << 1;      \
    if (a->nodes_allocated <= previous_length) {                               \
      if (previous_length < GARRAY_MAX_VALUE)                                  \
        a->nodes_allocated = GARRAY_MAX_VALUE;                                 \
      else {                                                                   \
        perror("___garray_check_resizing" #DATA_TYPE                           \
               "(): posible overflow of the garray_index type, try setting "   \
               "it to a bigger data type\n");                                  \
        abort();                                                               \
      }                                                                        \
    }                                                                          \
                                                                               \
    struct ___generic_garray_node_##DATA_TYPE *new_nodes = realloc(            \
        a->nodes, a->nodes_allocated *                                         \
                      sizeof(struct ___generic_garray_node_##DATA_TYPE));      \
                                                                               \
    if (new_nodes == NULL) {                                                   \
      free(a->nodes);                                                          \
      abort();                                                                 \
    }                                                                          \
                                                                               \
    a->nodes = new_nodes;                                                      \
                                                                               \
    for (garray_index i = previous_length; i < (a->nodes_allocated); i++)      \
      a->nodes[i].values_setted = 0;                                           \
                                                                               \
    return true;                                                               \
  }                                                                            \
                                                                               \
  DATA_TYPE *___garray_get_element##DATA_TYPE(garray_##DATA_TYPE a,            \
                                              garray_index position) {         \
    return &a->nodes[position >> ___GARRAY_LOG_B2_ELEMENTS_PER_NODE]           \
                .elements[position % ___GARRAY_ELEMENTS_PER_NODE];             \
  }                                                                            \
                                                                               \
  static garray_index ___garray_get_next_free##DATA_TYPE(                      \
      garray_##DATA_TYPE a) {                                                  \
    ___garray_check_resizing##DATA_TYPE(a);                                    \
                                                                               \
    if (!___GARRAY_GET_VALUE_SETTED(a->nodes, a->next_free))                   \
      return a->next_free;                                                     \
                                                                               \
    while (___GARRAY_GET_VALUE_SETTED(a->nodes, a->next_free)) {               \
      a->next_free++;                                                          \
      ___garray_check_resizing##DATA_TYPE(a);                                  \
    }                                                                          \
                                                                               \
    return a->next_free;                                                       \
  }                                                                            \
                                                                               \
  garray_index garray_##DATA_TYPE##_add(garray_##DATA_TYPE a,                  \
                                        DATA_TYPE data) {                      \
    garray_index pos = ___garray_get_next_free##DATA_TYPE(a);                  \
                                                                               \
    *___garray_get_element##DATA_TYPE(a, pos) = data;                          \
    ___GARRAY_SET_VALUE_SETTED(a->nodes, pos);                                 \
                                                                               \
    a->num_elements++;                                                         \
                                                                               \
    return pos;                                                                \
  }                                                                            \
                                                                               \
  DATA_TYPE const *garray_##DATA_TYPE##_at(garray_##DATA_TYPE a,               \
                                           garray_index position) {            \
    if (position >= a->nodes_allocated << ___GARRAY_LOG_B2_ELEMENTS_PER_NODE)  \
      abort();                                                                 \
                                                                               \
    if (!___GARRAY_GET_VALUE_SETTED(a->nodes, position))                       \
      abort();                                                                 \
                                                                               \
    return ___garray_get_element##DATA_TYPE(a, position);                      \
  }                                                                            \
                                                                               \
  DATA_TYPE const *garray_##DATA_TYPE##_at_default(                            \
      garray_##DATA_TYPE a, garray_index position,                             \
      DATA_TYPE const *default_value) {                                        \
                                                                               \
    if (position >= a->nodes_allocated << ___GARRAY_LOG_B2_ELEMENTS_PER_NODE)  \
      return default_value;                                                    \
                                                                               \
    if (!___GARRAY_GET_VALUE_SETTED(a->nodes, position))                       \
      return default_value;                                                    \
                                                                               \
    return ___garray_get_element##DATA_TYPE(a, position);                      \
  }                                                                            \
                                                                               \
  void garray_##DATA_TYPE##_set(garray_##DATA_TYPE a, garray_index position,   \
                                DATA_TYPE data) {                              \
    if (a->nodes_allocated << ___GARRAY_LOG_B2_ELEMENTS_PER_NODE < position) { \
      garray_index old_next_free = a->next_free;                               \
      a->next_free = position;                                                 \
                                                                               \
      while (___garray_check_resizing##DATA_TYPE(a))                           \
        ;                                                                      \
                                                                               \
      a->next_free = old_next_free;                                            \
                                                                               \
      a->num_elements++;                                                       \
    }                                                                          \
                                                                               \
    *___garray_get_element##DATA_TYPE(a, position) = data;                     \
    ___GARRAY_SET_VALUE_SETTED(a->nodes, position);                            \
  }                                                                            \
                                                                               \
  void garray_##DATA_TYPE##_remove(garray_##DATA_TYPE a,                       \
                                   garray_index position) {                    \
    ___GARRAY_UNSET_VALUE_SETTED(a->nodes, position);                          \
    a->num_elements--;                                                         \
                                                                               \
    if (position < a->next_free)                                               \
      a->next_free = position;                                                 \
  }                                                                            \
                                                                               \
  garray_index garray_##DATA_TYPE##_size(garray_##DATA_TYPE a) {               \
    return a->num_elements;                                                    \
  }                                                                            \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_clone(garray_##DATA_TYPE a) {        \
                                                                               \
    garray_##DATA_TYPE new_a =                                                 \
        garray_##DATA_TYPE##_new_prealocated(a->nodes_allocated);              \
                                                                               \
    new_a->nodes_allocated = a->nodes_allocated;                               \
    new_a->num_elements = a->num_elements;                                     \
    new_a->next_free = a->next_free;                                           \
                                                                               \
    memcpy(new_a->nodes, a->nodes,                                             \
           a->nodes_allocated *                                                \
               sizeof(struct ___generic_garray_node_##DATA_TYPE));             \
                                                                               \
    return new_a;                                                              \
  }                                                                            \
                                                                               \
  void garray_##DATA_TYPE##_collapse(garray_##DATA_TYPE a) {                   \
    garray_index tail =                                                        \
        (a->nodes_allocated << ___GARRAY_LOG_B2_ELEMENTS_PER_NODE) - 1;        \
                                                                               \
    for (garray_index head = 0; head < tail; head++) {                         \
      if (___GARRAY_GET_VALUE_SETTED(a->nodes, head))                          \
        continue;                                                              \
                                                                               \
      while (!___GARRAY_GET_VALUE_SETTED(a->nodes, tail)) {                    \
        if (head == --tail)                                                    \
          goto garray_##DATA_TYPE##_collapse_break_loop;                       \
      }                                                                        \
                                                                               \
      *___garray_get_element##DATA_TYPE(a, head) =                             \
          *___garray_get_element##DATA_TYPE(a, tail);                          \
                                                                               \
      ___GARRAY_UNSET_VALUE_SETTED(a->nodes, tail);                            \
      ___GARRAY_SET_VALUE_SETTED(a->nodes, head);                              \
    }                                                                          \
                                                                               \
    garray_##DATA_TYPE##_collapse_break_loop :;                                \
                                                                               \
    /* If the last element is unset, do not count it */                        \
    a->next_free =                                                             \
        ___GARRAY_GET_VALUE_SETTED(a->nodes, tail) ? tail + 1 : tail;          \
                                                                               \
    a->nodes_allocated =                                                       \
        (a->next_free >> ___GARRAY_LOG_B2_ELEMENTS_PER_NODE) + 1;              \
                                                                               \
    if (a->nodes_allocated == 0) {                                             \
      free(a->nodes);                                                          \
      a->nodes = NULL;                                                         \
    } else {                                                                   \
      struct ___generic_garray_node_##DATA_TYPE *new_nodes = realloc(          \
          a->nodes, sizeof(struct ___generic_garray_node_##DATA_TYPE) *        \
                        a->nodes_allocated);                                   \
                                                                               \
      if (new_nodes == NULL) {                                                 \
        free(a->nodes);                                                        \
        perror("garray_" #DATA_TYPE "_collapse(): realloc\n");                 \
        abort();                                                               \
      }                                                                        \
                                                                               \
      a->nodes = new_nodes;                                                    \
    }                                                                          \
  }                                                                            \
                                                                               \
  static void ___garray_sort_recursive##DATA_TYPE(                             \
      garray_index size, DATA_TYPE *data_array,                                \
      int criteria(DATA_TYPE const *, DATA_TYPE const *)) {                    \
    if (size < 3) {                                                            \
      if (size < 2)                                                            \
        return;                                                                \
                                                                               \
      if (criteria(&data_array[0], &data_array[1]) >= 0)                       \
        return;                                                                \
                                                                               \
      DATA_TYPE tmp = data_array[0];                                           \
      data_array[0] = data_array[1];                                           \
      data_array[1] = tmp;                                                     \
    }                                                                          \
                                                                               \
    garray_index pivot_position = size >> 1;                                   \
    DATA_TYPE *pivot = &data_array[pivot_position];                            \
                                                                               \
    garray_index left_size = 0;                                                \
    garray_index right_size = 0;                                               \
    DATA_TYPE left_array[size];                                                \
    DATA_TYPE right_array[size];                                               \
                                                                               \
    for (garray_index i = 0; i < pivot_position; i++) {                        \
      if (criteria(&data_array[i], pivot) > 0)                                 \
        left_array[left_size++] = data_array[i];                               \
      else                                                                     \
        right_array[right_size++] = data_array[i];                             \
    }                                                                          \
                                                                               \
    for (garray_index i = pivot_position + 1; i < size; i++) {                 \
      if (criteria(&data_array[i], pivot) >= 0) /* We do >= to balance out */  \
        left_array[left_size++] = data_array[i];                               \
      else                                                                     \
        right_array[right_size++] = data_array[i];                             \
    }                                                                          \
                                                                               \
    ___garray_sort_recursive##DATA_TYPE(left_size, left_array, criteria);      \
    ___garray_sort_recursive##DATA_TYPE(right_size, right_array, criteria);    \
                                                                               \
    memcpy(data_array, left_array,                                             \
           left_size * sizeof(DATA_TYPE)); /*Left array*/                      \
    data_array[left_size] = *pivot;        /* Pivot */                         \
    memcpy(&data_array[left_size + 1], right_array,                            \
           right_size * sizeof(DATA_TYPE)); /*Right array*/                    \
  }                                                                            \
                                                                               \
  void garray_##DATA_TYPE##_sort(                                              \
      garray_##DATA_TYPE a,                                                    \
      int criteria(DATA_TYPE const *, DATA_TYPE const *)) {                    \
    garray_##DATA_TYPE##_collapse(a);                                          \
                                                                               \
    DATA_TYPE array[a->num_elements];                                          \
                                                                               \
    for (garray_index i = 0; i < a->num_elements; i++) {                       \
      array[i] = *___garray_get_element##DATA_TYPE(a, i);                      \
    }                                                                          \
    ___garray_sort_recursive##DATA_TYPE(a->num_elements, array, criteria);     \
                                                                               \
    for (garray_index i = 0; i < a->num_elements; i++) {                       \
      *___garray_get_element##DATA_TYPE(a, i) = array[i];                      \
    }                                                                          \
  }                                                                            \
                                                                               \
  void garray_##DATA_TYPE##_free(garray_##DATA_TYPE a) {                       \
                                                                               \
    if (a->nodes != NULL)                                                      \
      free(a->nodes);                                                          \
                                                                               \
    free(a);                                                                   \
  }                                                                            \
                                                                               \
  garray_##DATA_TYPE##_iter garray_##DATA_TYPE##_iter_new(                     \
      garray_##DATA_TYPE a) {                                                  \
                                                                               \
    garray_##DATA_TYPE##_iter new_iter =                                       \
        malloc(sizeof(struct ___generic_garray_##DATA_TYPE##_iterator));       \
                                                                               \
    if (new_iter == NULL) {                                                    \
      perror("garray_" #DATA_TYPE "_iter_new_(): malloc\n");                   \
      abort();                                                                 \
    }                                                                          \
                                                                               \
    new_iter->garray = a;                                                      \
    new_iter->index = 0;                                                       \
    new_iter->valid_index =                                                    \
        a->nodes == NULL ? false : ___GARRAY_GET_VALUE_SETTED(a->nodes, 0);    \
                                                                               \
    return new_iter;                                                           \
  }                                                                            \
                                                                               \
  bool garray_##DATA_TYPE##_iter_condition(                                    \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    return iterator->valid_index;                                              \
  }                                                                            \
                                                                               \
  void garray_##DATA_TYPE##_iter_free(garray_##DATA_TYPE##_iter iterator) {    \
    free(iterator);                                                            \
  }                                                                            \
                                                                               \
  bool garray_##DATA_TYPE##_iter_condition_free(                               \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    if (garray_##DATA_TYPE##_iter_condition(iterator))                         \
      return true;                                                             \
                                                                               \
    garray_##DATA_TYPE##_iter_free(iterator);                                  \
    return false;                                                              \
  }                                                                            \
                                                                               \
  void garray_##DATA_TYPE##_iter_next(garray_##DATA_TYPE##_iter iterator) {    \
    while (++iterator->index < iterator->garray->nodes_allocated               \
                                   << ___GARRAY_LOG_B2_ELEMENTS_PER_NODE) {    \
      if (___GARRAY_GET_VALUE_SETTED(iterator->garray->nodes,                  \
                                     iterator->index)) {                       \
        iterator->valid_index = true;                                          \
        return;                                                                \
      }                                                                        \
    }                                                                          \
                                                                               \
    iterator->valid_index = false;                                             \
  }                                                                            \
                                                                               \
  void garray_##DATA_TYPE##_iter_previous(                                     \
      garray_##DATA_TYPE##_iter iterator) {                                    \
                                                                               \
    /* index has an offset of +1,                   */                         \
    /* so that when it reaches 0 that would be -1,  */                         \
    /* because whe are using unsigned types         */                         \
    while (iterator->index-- > 0) {                                            \
      if (___GARRAY_GET_VALUE_SETTED(iterator->garray->nodes,                  \
                                     iterator->index)) {                       \
        iterator->valid_index = true;                                          \
        return;                                                                \
      }                                                                        \
    }                                                                          \
                                                                               \
    iterator->valid_index = false;                                             \
  }                                                                            \
                                                                               \
  DATA_TYPE const *garray_##DATA_TYPE##_iter_get(                              \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    return ___garray_get_element##DATA_TYPE(iterator->garray,                  \
                                            iterator->index);                  \
  }                                                                            \
                                                                               \
  void garray_##DATA_TYPE##_iter_set(garray_##DATA_TYPE##_iter iterator,       \
                                     DATA_TYPE data) {                         \
    garray_##DATA_TYPE##_set(iterator->garray, iterator->index, data);         \
  }                                                                            \
                                                                               \
  garray_index garray_##DATA_TYPE##_iter_get_index(                            \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    return iterator->index;                                                    \
  }                                                                            \
                                                                               \
  bool garray_##DATA_TYPE##_iter_set_index(garray_##DATA_TYPE##_iter iterator, \
                                           garray_index index) {               \
    if (index >= iterator->garray->nodes_allocated                             \
                     << ___GARRAY_LOG_B2_ELEMENTS_PER_NODE)                    \
      return false;                                                            \
                                                                               \
    iterator->valid_index =                                                    \
        ___GARRAY_GET_VALUE_SETTED(iterator->garray->nodes, iterator->index);  \
                                                                               \
    iterator->index = index;                                                   \
    return true;                                                               \
  }                                                                            \
                                                                               \
  bool garray_##DATA_TYPE##_contains(                                          \
      garray_##DATA_TYPE a, DATA_TYPE value,                                   \
      bool comparator(DATA_TYPE const *left, DATA_TYPE const *right)) {        \
    garray_##DATA_TYPE##_iter iter;                                            \
                                                                               \
    for (iter = garray_##DATA_TYPE##_iter_new(a);                              \
         garray_##DATA_TYPE##_iter_condition_free(iter);                       \
         garray_##DATA_TYPE##_iter_next(iter)) {                               \
      if (comparator(&value, garray_##DATA_TYPE##_iter_get(iter)))             \
        goto garray_##DATA_TYPE##_contains_break_loop;                         \
    }                                                                          \
                                                                               \
    return false;                                                              \
                                                                               \
    garray_##DATA_TYPE##_contains_break_loop :;                                \
    garray_##DATA_TYPE##_iter_free(iter);                                      \
    return true;                                                               \
  }                                                                            \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_query(                               \
      garray_##DATA_TYPE a, void *data,                                        \
      bool condition(DATA_TYPE const *value, void *data)) {                    \
    garray_##DATA_TYPE new_a = garray_##DATA_TYPE##_new();                     \
    DATA_TYPE const *current = NULL;                                           \
                                                                               \
    for (garray_##DATA_TYPE##_iter it = garray_##DATA_TYPE##_iter_new(a);      \
         garray_##DATA_TYPE##_iter_condition_free(it);                         \
         garray_##DATA_TYPE##_iter_next(it)) {                                 \
      current = garray_##DATA_TYPE##_iter_get(it);                             \
                                                                               \
      if (condition(current, data))                                            \
        garray_##DATA_TYPE##_add(new_a, *current);                             \
    }                                                                          \
                                                                               \
    return new_a;                                                              \
  }                                                                            \
                                                                               \
  DATA_TYPE const *garray_##DATA_TYPE##_get(                                   \
      garray_##DATA_TYPE a, void *data,                                        \
      bool condition(DATA_TYPE const *value, void *data)) {                    \
    DATA_TYPE const *current = NULL;                                           \
    garray_##DATA_TYPE##_iter it;                                              \
                                                                               \
    for (it = garray_##DATA_TYPE##_iter_new(a);                                \
         garray_##DATA_TYPE##_iter_condition(it);                              \
         garray_##DATA_TYPE##_iter_next(it)) {                                 \
      current = garray_##DATA_TYPE##_iter_get(it);                             \
                                                                               \
      if (condition(current, data)) {                                          \
        garray_##DATA_TYPE##_iter_free(it);                                    \
        return current;                                                        \
      }                                                                        \
    }                                                                          \
                                                                               \
    garray_##DATA_TYPE##_iter_free(it);                                        \
    return NULL;                                                               \
  }

#endif

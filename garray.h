#ifndef GENERIC_ARRAY
#define GENERIC_ARRAY

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GARRAY_VERSION 2.0

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
 * garray_TYPE garray_TYPE_new_preallocated(garray_index
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
 * Returns a collapsed and sorted version of the input array according to
 * criteria
 * `criteria` == 0: None is before the other
 * `criteria` > 0: Left is before right
 * `criteria` < 0: Left is after right
 * garray_TYPE garray_TYPE_sort(garray_TYPE a,
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

typedef struct generic_array *garray;
typedef struct generic_array_iterator *garray_iter;

// Declare the array of type DATA_TYPE to use it when the array for that type
// has already been implemented at some other place
#define GARRAY_DECLARE(DATA_TYPE)                                              \
  typedef struct generic_array *garray_##DATA_TYPE;                            \
  typedef struct generic_array_iter *garray_##DATA_TYPE##_iter;                \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_new();                               \
                                                                               \
  garray_##DATA_TYPE garray_##DATA_TYPE##_new_preallocated(                    \
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
  garray_##DATA_TYPE garray_##DATA_TYPE##_sort(                                \
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

// Implement the array for the type DATA_TYPE --------------------------------
#define GARRAY_IMPLEMENT(DATA_TYPE)                                            \
  typedef struct generic_array *garray_##DATA_TYPE;                            \
  typedef struct generic_array_iter *garray_##DATA_TYPE##_iter;                \
                                                                               \
  garray ___garray_new(garray_index element_size);                             \
  garray ___garray_new_preallocated(garray_index num_elements_preallocated,    \
                                    garray_index element_size);                \
  garray_index ___garray_add(garray a, const void *data);                      \
  const void *___garray_at(garray a, garray_index position);                   \
  const void *___garray_at_default(garray a, garray_index position,            \
                                   const void *default_value);                 \
  void ___garray_set(garray a, garray_index position,                          \
                     const void *restrict data);                               \
  void ___garray_remove(garray a, garray_index position);                      \
  garray_index ___garray_size(garray a);                                       \
  garray ___garray_clone(garray a);                                            \
  void ___garray_collapse(garray a);                                           \
  garray ___garray_sort(garray a, int criteria(void const *, void const *));   \
  void ___garray_free(garray a);                                               \
  garray_iter ___garray_iter_new(garray a);                                    \
  bool ___garray_iter_condition(garray_iter iterator);                         \
  void ___garray_iter_free(garray_iter iterator);                              \
  bool ___garray_iter_condition_free(garray_iter iterator);                    \
  void ___garray_iter_next(garray_iter iterator);                              \
  void ___garray_iter_previous(garray_iter iterator);                          \
  void const *___garray_iter_get(garray_iter iterator);                        \
  void ___garray_iter_set(garray_iter iterator, const void *data);             \
  garray_index ___garray_iter_get_index(garray_iter iterator);                 \
  bool ___garray_iter_set_index(garray_iter iterator, garray_index index);     \
  bool ___garray_contains(                                                     \
      garray a, const void *value,                                             \
      bool comparator(void const *left, void const *right));                   \
  garray ___garray_query(garray a, void *data,                                 \
                         bool condition(void const *value, void *data));       \
  void const *___garray_get(garray a, void *data,                              \
                            bool condition(void const *value, void *data));    \
                                                                               \
  extern inline garray_##DATA_TYPE garray_##DATA_TYPE##_new() {                \
    return ___garray_new(sizeof(DATA_TYPE));                                   \
  }                                                                            \
                                                                               \
  extern inline garray_##DATA_TYPE garray_##DATA_TYPE##_new_preallocated(      \
      garray_index num_elements_preallocated) {                                \
    return ___garray_new_preallocated(num_elements_preallocated,               \
                                      sizeof(DATA_TYPE));                      \
  }                                                                            \
                                                                               \
  extern inline garray_index garray_##DATA_TYPE##_add(garray_##DATA_TYPE a,    \
                                                      DATA_TYPE data) {        \
    return ___garray_add(a, &data);                                            \
  }                                                                            \
                                                                               \
  extern inline DATA_TYPE const *garray_##DATA_TYPE##_at(                      \
      garray_##DATA_TYPE a, garray_index position) {                           \
    return ___garray_at(a, position);                                          \
  }                                                                            \
                                                                               \
  extern inline DATA_TYPE const *garray_##DATA_TYPE##_at_default(              \
      garray_##DATA_TYPE a, garray_index position,                             \
      DATA_TYPE const *default_value) {                                        \
    return ___garray_at_default(a, position, default_value);                   \
  }                                                                            \
                                                                               \
  extern inline void garray_##DATA_TYPE##_set(                                 \
      garray_##DATA_TYPE a, garray_index position, DATA_TYPE data) {           \
    ___garray_set(a, position, &data);                                         \
  }                                                                            \
                                                                               \
  extern inline void garray_##DATA_TYPE##_remove(garray_##DATA_TYPE a,         \
                                                 garray_index position) {      \
    ___garray_remove(a, position);                                             \
  }                                                                            \
                                                                               \
  extern inline garray_index garray_##DATA_TYPE##_size(garray_##DATA_TYPE a) { \
    return ___garray_size(a);                                                  \
  }                                                                            \
                                                                               \
  extern inline garray_##DATA_TYPE garray_##DATA_TYPE##_clone(                 \
      garray_##DATA_TYPE a) {                                                  \
    return ___garray_clone(a);                                                 \
  }                                                                            \
                                                                               \
  extern inline void garray_##DATA_TYPE##_collapse(garray_##DATA_TYPE a) {     \
    ___garray_collapse(a);                                                     \
  }                                                                            \
                                                                               \
  extern inline garray_##DATA_TYPE garray_##DATA_TYPE##_sort(                  \
      garray_##DATA_TYPE a,                                                    \
      int criteria(DATA_TYPE const *, DATA_TYPE const *)) {                    \
    return ___garray_sort(a, (int (*)(void const *, void const *))criteria);   \
  }                                                                            \
                                                                               \
  extern inline void garray_##DATA_TYPE##_free(garray_##DATA_TYPE a) {         \
    ___garray_free(a);                                                         \
  }                                                                            \
                                                                               \
  extern inline garray_##DATA_TYPE##_iter garray_##DATA_TYPE##_iter_new(       \
      garray_##DATA_TYPE a) {                                                  \
    return (garray_##DATA_TYPE##_iter)___garray_iter_new(a);                   \
  }                                                                            \
                                                                               \
  extern inline bool garray_##DATA_TYPE##_iter_condition(                      \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    return ___garray_iter_condition((garray_iter)iterator);                    \
  }                                                                            \
                                                                               \
  extern inline void garray_##DATA_TYPE##_iter_free(                           \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    ___garray_iter_free((garray_iter)iterator);                                \
  }                                                                            \
                                                                               \
  extern inline bool garray_##DATA_TYPE##_iter_condition_free(                 \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    return ___garray_iter_condition_free((garray_iter)iterator);               \
  }                                                                            \
                                                                               \
  extern inline void garray_##DATA_TYPE##_iter_next(                           \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    ___garray_iter_next((garray_iter)iterator);                                \
  }                                                                            \
                                                                               \
  extern inline void garray_##DATA_TYPE##_iter_previous(                       \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    ___garray_iter_previous((garray_iter)iterator);                            \
  }                                                                            \
                                                                               \
  extern inline DATA_TYPE const *garray_##DATA_TYPE##_iter_get(                \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    return ___garray_iter_get((garray_iter)iterator);                          \
  }                                                                            \
                                                                               \
  extern inline void garray_##DATA_TYPE##_iter_set(                            \
      garray_##DATA_TYPE##_iter iterator, DATA_TYPE data) {                    \
    ___garray_iter_set((garray_iter)iterator, (void const *)&data);            \
  }                                                                            \
                                                                               \
  extern inline garray_index garray_##DATA_TYPE##_iter_get_index(              \
      garray_##DATA_TYPE##_iter iterator) {                                    \
    return ___garray_iter_get_index((garray_iter)iterator);                    \
  }                                                                            \
                                                                               \
  extern inline bool garray_##DATA_TYPE##_iter_set_index(                      \
      garray_##DATA_TYPE##_iter iterator, garray_index index) {                \
    return ___garray_iter_set_index((garray_iter)iterator, index);             \
  }                                                                            \
                                                                               \
  extern inline bool garray_##DATA_TYPE##_contains(                            \
      garray_##DATA_TYPE a, DATA_TYPE value,                                   \
      bool comparator(DATA_TYPE const *left, DATA_TYPE const *right)) {        \
    return ___garray_contains(                                                 \
        a, &value, (bool (*)(void const *, void const *))comparator);          \
  }                                                                            \
                                                                               \
  extern inline garray_##DATA_TYPE garray_##DATA_TYPE##_query(                 \
      garray_##DATA_TYPE a, void *data,                                        \
      bool condition(DATA_TYPE const *value, void *data)) {                    \
    return ___garray_query(a, data,                                            \
                           (bool (*)(void const *, void *))condition);         \
  }                                                                            \
                                                                               \
  extern inline DATA_TYPE const *garray_##DATA_TYPE##_get(                     \
      garray_##DATA_TYPE a, void *data,                                        \
      bool condition(DATA_TYPE const *value, void *data)) {                    \
    return ___garray_get(a, data, (bool (*)(void const *, void *))condition);  \
  }

#endif

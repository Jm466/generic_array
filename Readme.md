# Generic Array Library

This library adds the functionaly of generics arrays.

## Table of contents

1. [Usage](#usage)
2. [Overview](#overview)
3. [Documentation](#documentation)
4. [Implementation details](#Implementation)

## Usage

You just copy `garray.h` in your project folder and that is it.

## Overview

It works by defining the `GARRAY_DECLARE()` and `GARRAY_IMPLEMENT()` macros.

`GARRAY_IMPLEMENT(DATA_TYPE)` expands to all the function implementations
required for an array of `DATA_TYPE`, `GARRAY_DECLARE(DATA_TYPE)` expands to a
declaration of all funcions required for an array of `DATA_TYPE`

You would use `GARRAY_IMPLEMENT(DATA_TYPE)` to implement an array of that type
and then if you need to use an array of that type in other module you would
use `GARRAY_DECLARE(DATA_TYPE)`

> **_IMPORTANT:_** `DATA_TYPE` must be a single word or else it may not work
> `GARRAY_DECLARE(unsigned int)` it is not allowed.
> You would need to do something like:
>
> ```c
> typedef unsigned int myType;
> GARRAY_DECLARE(myType)
> ```

> **_NOTE:_** You may use `GARRAY_DECLARE(DATA_TYPE)` multiple times per type,
> but you may only use `GARRAY_IMPLEMENT(DATA_TYPE)` only once per type

## Documentation

Now follows the documentation for every function in the library, note that
the function name changes based on the type:

`garray_new` → `garray_int_new`, `garray_char_new`, etc.

The types `garray` and `garray_iter` change also to `garray_int_iter` and
`garray_char_iter` respectively. The following documentation assumes that the
type is `TYPE`.

---

```c
garray_TYPE garray_TYPE_new();
```

Returns an empty new array of type `TYPE`

---

```c
garray_TYPE garray_TYPE_new_prealocated(garray_index num_elements_preallocated)
```

Same as `garray_TYPE_new()` but the array start with a size of `num_elements_preallocated`

---

```c
garray_index garray_TYPE_add(garray_TYPE a, TYPE data);
```

Appends an element to the array

---

```c
TYPE const *garray_TYPE_at(garray_TYPE a, garray_index position);
```

Returns an unmodifiable pointer to the value at `position`.
Returns aborts the program if `position` its outside of bounds or the value is uset.

---

```c
TYPE const *garray_TYPE_at_default(garray_TYPE a, garray_index position, TYPE const *default_value);
```

Returns an unmodifiable pointer to the value at `position`.
Returns `default` if if `position` its outside of bounds or the value is unset.

---

```c
TYPE const _garray_TYPE_get(garray_TYPE a, void *data, bool condition(TYPE const _value, void_ data))
```

Returns an unmodifiable pointer to the first value that matches `condition`, `NULL` if none matches.
`data` This value will be passed to condition each time its called

---

```c
garray_TYPE garray_TYPE_query(garray_TYPE a, void* data, bool condition(TYPE const *value, void\* data))
```

Returns a new garray that contains all the elements that matches `condition`.

---

```c
void garray_TYPE_set(garray_TYPE a, garray_index position, TYPE data);
```

Sets the value at `position`, will increment the allocated space for the array if necessary

---

```c
void garray_TYPE_remove(garray_TYPE a, garray_index position);
```

Unsets the value at `position`.

> **_NOTE:_** When iterating, unsetted values will be ignored.
> Also, `garray_TYPE_at()` will return `NULL` when trying to get at a non setted position.

---

```c
garray_index garray_TYPE_size(garray_TYPE a);
```

Returns the number of elements of the array

---

```c
garray_TYPE garray_TYPE_clone(garray_TYPE a);
```

Returns an exact copy of the array. Iterators of the original array are not affected

---

```c
void garray_TYPE_collapse(garray_TYPE a);
```

It gets rid of the unsetted values in the array reducing the allocated space
to the number of elements in the array

---

```c
void garray_TYPE_sort(garray_TYPE a, int criteria(TYPE const *left, TYPE const *right));
```

Collapses and sorts the array according to criteria
`criteria` == 0: None is before the other
`criteria` > 0: Left is before right
`criteria` < 0: Left is after right

---

```c
bool garray_TYPE_contains(garray_TYPE a, TYPE value, bool comparator(TYPE const *left, TYPE const *right))
```

Returns true if value is contained in the array, according to comparator

---

```c
void garray_TYPE_free(garray_TYPE a);
```

Frees the array

---

```c
garray_TYPE_iter garray_TYPE_iter_new(garray_TYPE a);
```

Returns a new iterator for the array
Iterators allow you to iterate easily over the array

> **_IMPORTANT:_** After creating a new iterator, modifiying the structure of the
> array by calling `garray_TYPE_collapse()`, `garray_TYPE_sort()` or any other
> function that alters the number of elements or its positions in the
> array(like `garray_TYPE_set()` when called with a position that would require
> the array to expand), can get the iterator into an inconsistent state that
> could lead to a crash

---

```c
bool garray_TYPE_iter_condition(garray_TYPE_iter iterator);
```

Returns false if the iterator is at the end of the array, true in other case

---

```c
bool garray_TYPE_iter_condition_free(garray_TYPE_iter_int iterator);
```

Same as garray_TYPE_iter_condition but when it reaches the end
automatically frees the iterator

---

```c
void garray_TYPE_iter_next(garray_TYPE_iter iterator);
```

Advances the iterator to the next available value

---

```c
void garray_TYPE_iter_previous(garray_TYPE_iter iterator);
```

Recedes the iterator to the previous available value

---

```c
TYPE *garray_TYPE_iter_get(garray_TYPE_iter iterator);
```

Returns a pointer to the value at the current iterator position

---

```c
TYPE *garray_TYPE_iter_set(garray_TYPE_iter iterator, TYPE data);
```

Sets the value at the current iterator position.

---

```c
garray_index garray_TYPE_iter_get_index(garray_TYPE_iter iterator);
```

Returns the index of the array in the current iterator position

---

```c
garray_index garray_TYPE_iter_set_index(garray_TYPE_iter iterator, garray_index index);
```

Sets the iterator position to `index`

Returns `true` if the index was setted, `false` otherwise

---

```c
void garray_TYPE_iter_free(garray_TYPE_iter_int iterator);
```

Frees the iterator

## Implementation

### The macros

The `GARRAY_DECLARE()` macro just expands to types and function declarations,
wich allows you to use it as much as you want throughtout the code, because there is
no actuall function implementation.

On the other hand, `GARRAY_IMPLEMENT` expands to the types and function declaration for
that type, calling this macro multiple times would cause a linker error, because you would
be redefining already declared functions.

### Data layout

The generic array itself is an array of `___generic_garray_node_DATA_TYPE`, defined as the following:

```c
struct ___generic_garray_node_TYPE {
    uint8_t values_setted;
    TYPE elements[___GARRAY_ELEMENTS_PER_NODE];
  };
```

Being `___GARRAY_ELEMENTS_PER_NODE` defined as 8

---

`values_setted` is a byte that contains in binary wich values of the node are setted;
that way the information about the state of a node is close to the value that holds the node,
so that the processor cache may take advantage of spacial locality

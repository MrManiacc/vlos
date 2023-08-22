#pragma once
#include "defines.h"

/**
* Memory layout
 * u64 capacity = number of elements that can be stored in the array
 * u64 length = number of elements currently in the array
 * u64 stride = size of each element in the array
 * void* data = pointer to the data
*/
enum {
  DARRAY_CAPACITY,
  DARRAY_LENGTH,
  DARRAY_STRIDE,
  DARRAY_FIELD_LENGTH
};
/**
 * Creates a new dynamic array with the given length and stride
 * @param length  The length of the array
 * @param stride The stride of each element in the array
 * @return a void pointer to the array
 */
VAPI void *_darray_create(u64 length, u64 stride);

/**
 * Destroys the given dynamic array
 * @param array the pointer to the array to destroy
 */
VAPI void _darray_destroy(void *array);

/**
 * Gets a pointer to the data of the given array.
 * @param array  The pointer to the array
 * @param field The field to get
 * @return a void pointer to the data
 */
VAPI u64 _darray_field_get(void *array, u64 field);

/**
 * Sets the given field of the array to the given value
 * @param array  The pointer to the array
 * @param field The field to set
 * @param value The value to set the field to
 */
VAPI void _darray_field_set(void *array, u64 field, u64 value);

/**
* Should be automatically resized according to what's needed
 * @param array  The pointer to the array
 * @return a void pointer to the array
*/
VAPI void *_darray_resize(void *array);

/**
* Pushes the given value to the end of the array
 * @param array  The pointer to the array
 * @param value The value to push
 * @return a void pointer to the array
*/
VAPI void *_darray_push(void *array, const void *value_ptr);

/**
* Pushes the given value to the end of the array
 * @param array  The pointer to the array
 * @param index The index to insert the value at
 * @param value The value to push
 * @return a void pointer to the array
*/
VAPI void *_darray_push_at(void *array, u64 index, void *value);

/**
* Pops the values into the given pointer
 * @param array  The pointer to the array
 * @param dest The destination to pop the value into
*/
VAPI void _darray_pop(void *array, void *dest);

/**
 * Pops the value at the given index into the given pointer
 * @param array The pointer to the array
 * @param index The index to insert the value at
 * @param dest The destination to pop the value into
 * @return a void pointer to the array
 */
VAPI void *_darray_pop_at(void *array, u64 index, void *dest);

#define DARRAY_DEFAULT_CAPACITY 1
#define DARRAY_RESIZE_FACTOR 2

#define darray_create(type) \
  _darray_create(DARRAY_DEFAULT_CAPACITY, sizeof(type))

#define darray_reserve(type, capacity) \
  _darray_create(capacity, sizeof(type))

#define darray_destroy(array) _darray_destroy(array);

#define darray_push(array, value)         \
    {                                     \
      typeof(value) temp = value;         \
      array = _darray_push(array, &temp); \
    }

#define darray_pop(array, dest) \
  _darray_pop(array, dest)

#define darray_push_at(array, index, value) \
    {                                       \
      typeof(value) temp = value;           \
      array = _darray_push_at(array, index, &temp); \
    }

#define darray_pop_at(array, index, dest) \
    _darray_pop_at(array, index, dest)

#define darray_clear(array) \
  _darray_field_set(array, DARRAY_LENGTH, 0)

#define darray_length(array) \
    _darray_field_get(array, DARRAY_LENGTH)

#define darray_capacity(array) \
    _darray_field_get(array, DARRAY_CAPACITY)

#define darray_stride(array) \
    _darray_field_get(array, DARRAY_STRIDE)

#define darray_set_length(array, length) \
    _darray_field_set(array, DARRAY_LENGTH, length)

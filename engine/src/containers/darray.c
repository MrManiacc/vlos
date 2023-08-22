#include "darray.h"
#include "core/logger.h"
#include "core/mem.h"

ptr _darray_create(u64
                   length,
                   u64 stride
) {
  u64 header_size = DARRAY_FIELD_LENGTH * sizeof(u64);
  u64 array_size = length * stride;
  u64 *new_array = mem_alloc(header_size + array_size, MEM_TAG_DARRAY);
  mem_set(new_array,
          0, header_size + array_size);
  new_array[DARRAY_CAPACITY] =
      length;
  new_array[DARRAY_LENGTH] = 0;
  new_array[DARRAY_STRIDE] =
      stride;
  return (ptr) (new_array + DARRAY_FIELD_LENGTH);
}

void _darray_destroy(void *array) {
  u64 *header = (u64 *) array - DARRAY_FIELD_LENGTH;
  u64 header_size = DARRAY_FIELD_LENGTH * sizeof(u64);
  u64 total_size = header_size + header[DARRAY_CAPACITY] * header[DARRAY_STRIDE];
  mem_free(header, total_size, MEM_TAG_DARRAY);
}

u64 _darray_field_get(void *array, u64 field) {
  u64 *header = (u64 *) array - DARRAY_FIELD_LENGTH;
  return header[field];
}

void _darray_field_set(void *array, u64 field, u64 value) {
  u64 *header = (u64 *) array - DARRAY_FIELD_LENGTH;
  header[field] = value;
}

void *_darray_resize(void *array) {
  u64 length = darray_length(array);
  u64 stride = darray_stride(array);
  void *temp = _darray_create(
      (DARRAY_RESIZE_FACTOR * darray_capacity(array)),
      stride);
  mem_copy(temp, array, length * stride);

  _darray_field_set(temp, DARRAY_LENGTH, length);
  _darray_destroy(array);
  return temp;
}

/**
* Pushes the given value to the end of the array
 * @param array  The pointer to the array
 * @param value The value to push
 * @return a void pointer to the array
*/
void *_darray_push(void *array, const void *value_ptr) {
  u64 length = darray_length(array);
  u64 stride = darray_stride(array);
  if (length >= darray_capacity(array)) {
    array = _darray_resize(array);
  }

  u64 addr = (u64) array;
  addr += (length * stride);
  mem_copy((void *) addr, value_ptr, stride);
  _darray_field_set(array, DARRAY_LENGTH, length + 1);
  return array;
}

/**
* Pushes the given value to the end of the array
 * @param array  The pointer to the array
 * @param index The index to insert the value at
 * @param value The value to push
 * @return a void pointer to the array
*/
void *_darray_push_at(void *array, u64 index, void *value) {
  u64 length = darray_length(array);
  u64 stride = darray_stride(array);
  if (index >= length) {
    verror("Index outside the bounds of this array! Length: %i, index: %index", length, index);
    return array;
  }
  if (length >= darray_capacity(array)) {
    array = _darray_resize(array);
  }

  u64 addr = (u64)array;

  // If not on the last element, copy the rest outward.
  if (index != length - 1) {
    mem_copy(
        (void*)(addr + ((index + 1) * stride)),
        (void*)(addr + (index * stride)),
        stride * (length - index));
  }

  // Set the value at the index
  mem_copy((void*)(addr + (index * stride)), value, stride);

  _darray_field_set(array, DARRAY_LENGTH, length + 1);
  return array;
}

/**
* Pops the values into the given pointer
 * @param array  The pointer to the array
 * @param dest The destination to pop the value into
*/
void _darray_pop(void *array, void *dest) {
  u64 length = darray_length(array);
  u64 stride = darray_stride(array);
  u64 addr = (u64) array;
  addr += ((length - 1) * stride);
  mem_copy(dest, (void *) addr, stride);
  _darray_field_set(array, DARRAY_LENGTH, length - 1);
}

/**
 * Pops the value at the given index into the given pointer
 * @param array The pointer to the array
 * @param index The index to insert the value at
 * @param dest The destination to pop the value into
 * @return a void pointer to the array
 */
void *_darray_pop_at(void *array, u64 index, void *dest) {
  u64 length = darray_length(array);
  u64 stride = darray_stride(array);
  if (index >= length) {
    verror("Index outside the bounds of this array! Length: %i, index: %index", length, index);
    return array;
  }

  u64 addr = (u64)array;
  mem_copy(dest, (void*)(addr + (index * stride)), stride);

  // If not on the last element, snip out the entry and copy the rest inward.
  if (index != length - 1) {
    mem_copy(
        (void*)(addr + (index * stride)),
        (void*)(addr + ((index + 1) * stride)),
        stride * (length - index));
  }

  _darray_field_set(array, DARRAY_LENGTH, length - 1);
  return array;
}


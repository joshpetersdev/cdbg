#include "cdbg/string_vec.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*********************************************************************
 * @brief Initializes a StringVec structure to an empty state.
 *
 * Sets the internal data pointer to NULL and resets size and capacity
 * to zero. Must be called before using the vector.
 *
 * @param vec Pointer to the StringVec to initialize.
 ********************************************************************/
void string_vec_init(StringVec* vec) {
  vec->data = NULL;
  vec->size = 0;
  vec->capacity = 0;
}

/**********************************************************************
 * @brief Appends a copy of a string to the vector.
 *
 * If the vector has reached capacity, the internal storage is resized.
 * The provided string is duplicated and stored internally.
 *
 * Exits the program if memory allocation fails.
 *
 * @param vec Pointer to the StringVec.
 * @param str Null-terminated string to append.
 ********************************************************************/
void string_vec_push(StringVec* vec, const char* str) {
  if (vec->size == vec->capacity) {
    size_t new_capacity = (vec->capacity == 0) ? 4 : vec->capacity * 2;

    char** new_data = realloc(vec->data, new_capacity * sizeof(char*));

    if (new_data == NULL) {
      fprintf(stderr, "Out of memory\n");
      exit(EXIT_FAILURE);
    }

    vec->data = new_data;
    vec->capacity = new_capacity;
  }

  char* copy = malloc(strlen(str) + 1);
  if (copy == NULL) {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }

  strcpy(copy, str);

  vec->data[vec->size] = copy;
  vec->size++;
}

/*****************************************************************
 * @brief Frees all memory owned by the vector.
 *
 * Releases each stored string, then frees the internal array.
 * Resets the vector to an empty state so it can be reused safely.
 *
 * @param vec Pointer to the StringVec to free.
 ****************************************************************/
void string_vec_free(StringVec* vec) {
  for (size_t i = 0; i < vec->size; i++) {
    free(vec->data[i]);
  }

  free(vec->data);

  vec->data = NULL;
  vec->size = 0;
  vec->capacity = 0;
}

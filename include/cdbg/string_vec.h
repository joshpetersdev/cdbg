#ifndef CDBG_STRING_VEC_H
#define CDBG_STRING_VEC_H

#include <stddef.h>

typedef struct {
  char** data;
  size_t size;
  size_t capacity;
} StringVec;

void string_vec_init(StringVec* vec);
void string_vec_push(StringVec* vec, const char* str);
void string_vec_free(StringVec* vec);

#endif

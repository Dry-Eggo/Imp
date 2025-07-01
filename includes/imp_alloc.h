#pragma once
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <imp_log.h>
#define IMP_ARENA_ALIGNMENT (sizeof(void *))

typedef struct {
  char *buffer;
  size_t capacity;
  size_t cursor;
} ImpArena;

ImpArena *imp_arena_init(size_t capacity);
void *imp_arena_alloc(ImpArena *arena, size_t size);
void imp_arena_reset(ImpArena *arena);
void imp_arena_free(ImpArena *arena);

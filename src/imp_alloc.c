#include <imp_alloc.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

size_t align_up(size_t v, size_t alignment)
{
    return (v + alignment - 1) & ~(alignment - 1);
}

ImpArena* imp_arena_init(size_t capacity)
{
    ImpArena* arena = (ImpArena*)malloc(sizeof(ImpArena));
    if (!arena)
    {
	imp_log("Null ImpArena");
	return NULL;
    }
    arena->buffer = (char*)malloc(capacity);
    if (!arena->buffer)
    {
	imp_log("Null ImpArena: Init ImpArena Buffer Failed");
	return NULL;
    }
    arena->capacity = capacity;
    arena->cursor   = 0;
    imp_log("ImpArena Initialized Successfully");
    return arena;
}

void* imp_arena_alloc(ImpArena* arena, size_t size)
{
    if (!arena)
    {
	imp_log("Null ImpArena: alloc failed");
	return NULL;
    }

    size_t aligned_current_pos = align_up(arena->cursor, IMP_ARENA_ALIGNMENT);
    size_t effective_size      = size;
    size_t new_pos             = aligned_current_pos + effective_size;
    if (new_pos > arena->capacity)
    {
	imp_log("ImpArena out of capacity: Reallocating");
	size_t new_cap = arena->capacity*2;
	arena->buffer  = (char*)realloc(arena->buffer, new_cap);
	if (!arena->buffer)
	{
	    imp_log("Reallocation Failed");
	    return NULL;
	}
	arena->capacity = new_cap;
    }
    void *ptr = (void*)(arena->buffer + aligned_current_pos);
    arena->cursor = new_pos;
    imp_log("Allocated %zu bytes at %zu offset. cursor: %zu", size, aligned_current_pos, arena->cursor);
    return ptr;
}

void  imp_arena_reset(ImpArena* arena)
{
    if (arena)
    {
	arena->cursor = 0;
	imp_log("ImpArena reset");
    }
}

void  imp_arena_free(ImpArena* arena)
{
    if (arena)
    {
	if (arena->buffer)
	{
	    free(arena->buffer);
	    imp_log("ImpArena Buffer freed");
	}
	free(arena);
	imp_log("ImpArena freed");
    }
}

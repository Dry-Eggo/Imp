#pragma once

#include <imp_alloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define IMP_EOF EOF
#define imp_move_once(ctx) imp_move(ctx, 1)
#define imp_is_ok(ctx) (imp_peek_char(ctx) != IMP_EOF)

typedef struct {
  size_t line;
  size_t col;
} ImpLoc;

typedef struct {
  const char *filename;
  char *source;
  size_t pos;
  size_t col;
  size_t line;
  ImpArena *arena;
} ImpCtx;

bool imp_context_from_file        (ImpCtx *, const char *path, size_t ac);
bool imp_match_char               (ImpCtx *, char ch);
bool imp_parse_word               (ImpCtx *, char *buffer);
bool imp_parse_until_word         (ImpCtx *, char* buffer, const char* end);
bool imp_parse_until_char         (ImpCtx *, char* buffer, char dl);
bool imp_parse_until_charf        (ImpCtx *, char dl, char* fmt,...);
bool imp_peek_word                (ImpCtx *, char *buffer);
bool imp_next_char_is_ws          (ImpCtx *);

char imp_peek_char                (ImpCtx *);

void imp_move                     (ImpCtx *ctx, size_t offset);
void imp_close_context            (ImpCtx *);

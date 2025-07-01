#include "imp_log.h"
#include <ctype.h>
#include <imp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool imp_context_from_file(ImpCtx *ctx, const char *path, size_t arena_capacity)
{
  ctx->filename = strdup(path);
  ctx->arena = imp_arena_init(arena_capacity);
  FILE *infile = fopen(path, "r");
  if (!infile)
    return false;
  int n = fseek(infile, 0, SEEK_END);
  size_t size = ftell(infile);
  ctx->source = (char *)imp_arena_alloc(ctx->arena, sizeof(char) * size);
  rewind(infile);
  fread(ctx->source, size, 1, infile);

  ctx->pos  = 0;
  ctx->line = 1;
  ctx->col  = 1;
  
  return true;
}

bool imp_match_char(ImpCtx *ctx, char ch) {
  char current_char = imp_peek_char(ctx);
  return (current_char == ch) ? true : false;
}

bool imp_parse_word(ImpCtx *ctx, char *buffer)
{
  if (imp_match_char(ctx, ' '))
  {
    imp_move_once(ctx);
    return imp_parse_word(ctx, buffer);
  }
  size_t inx = 0;
  while (!imp_next_char_is_ws(ctx))
  {
    if (imp_peek_char(ctx) == IMP_EOF) return false;
    buffer[inx++] = imp_peek_char(ctx);
    imp_move_once(ctx);
  }
  buffer[inx] = '\0';
  return true;
}

bool imp_next_char_is_ws(ImpCtx * ctx)
{
  return isspace(imp_peek_char(ctx));
}

bool imp_parse_until_char(ImpCtx * ctx, char *buffer, char dl)
{
  size_t inx = 0;
  while (!imp_match_char(ctx, dl))
  {
    if (imp_peek_char(ctx) == IMP_EOF) return false;
    buffer[inx++] = imp_peek_char(ctx);
    imp_move_once(ctx);
  }
  buffer[inx] = '\0';
  return true;
}


bool imp_parse_until_charf(ImpCtx *ctx, char dl, char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int fmt_inx = 0;
  int fmt_max = strlen(fmt);
  char token[256];
  if (!imp_is_ok(ctx)) return false;
  for(int i = 0; i < fmt_max && imp_is_ok(ctx);)
  {
    char f = fmt[i];
    if (imp_match_char(ctx, dl))
      break;
    if (imp_match_char(ctx, f))
    {
      i++;
      imp_move_once(ctx);
      continue;
    } else if (f == '%')
    {
      i++;
      if (fmt[i] == 's')
      {
        i++;
        char* out = va_arg(args, char*);
        imp_parse_word(ctx, out);
        printf("ctx char = %c, fmt char = %c\n", imp_peek_char(ctx), fmt[i]);
        continue;
      }
    } else
    {
      imp_log("Match Terminated at '%c' '%c'", fmt[i], imp_peek_char(ctx));
      return false;      
    }
 }
 if (imp_peek_char(ctx) == dl)
     imp_move_once(ctx);
  return true;
}

char imp_peek_char(ImpCtx *ctx)
{
  if (ctx->pos >= strlen(ctx->source))
  {
    return IMP_EOF;
  }
  return ctx->source[ctx->pos];
}

void imp_move(ImpCtx *ctx, size_t offset)
{
    for (int i = ctx->pos; i < ctx->pos + offset; i++)
    {
      char ch = ctx->source[ctx->pos];
      if (ch == '\n')
      {
        ctx->line++;
        ctx->col  = 1;
      }
      else
      {
        ctx->col++;
      }
    }
    ctx->pos += offset;
}

void imp_close_context(ImpCtx *ctx) { imp_arena_free(ctx->arena); }

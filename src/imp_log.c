#include <imp_log.h>

static int logging_disabled = 0;
void imp_toggle_logging()
{
  logging_disabled = 1 - logging_disabled;
}
void imp_log(const char* fmt, ...)
{
  if (logging_disabled) return;
    va_list args;
    va_start(args, fmt);
    printf("[IMP_LOG]: ");
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

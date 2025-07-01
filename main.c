#include "imp_log.h"
#include <stdio.h>
#include <imp.h>

int main(void)
{
    ImpCtx ctx;
    const char* path = "examples/test.cfg";
    imp_context_from_file(&ctx, path, 1024);
    char key[64], value[64];
    while (imp_parse_until_charf(&ctx, '\n', "%s = %s", key, value))
    {
        printf("Key: '%s', Value = '%s'\n", key, value);
    }
        
    imp_close_context(&ctx);
    return 0;
}

#pragma once


#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>


void imp_toggle_logging();
void imp_log(const char* fmt, ...);

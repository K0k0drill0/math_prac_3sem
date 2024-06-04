#ifndef _SETTINGS_WORK
#define _SETTINGS_WORK

#define _GNU_SOURCE
#include "interpreter.h"
#include <string.h>

status_code read_file_with_settings(FILE *file, Current_settings* settings);

status_code write_new_settings(Current_settings* settings);

#endif
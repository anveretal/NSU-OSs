#include <stdio.h>
#include "config.h"

static int is_config_initialized = 0;

int create_config_table(void) {
    if (is_config_initialized) {
        return 1;
    }
    is_config_initialized = 1;
    printf("Config initialized\n");
    return 0;
}

int destroy_config_table(void) {
    if (!is_config_initialized) {
        return 1;
    }
    is_config_initialized = 0;
    printf("Config destroyed\n");
    return 0;
}
#include "plugin_api.h"

static double PLUGIN_CALL add_apply(const double* a, unsigned int n) {
    return a[0] + a[1];
}

const char *help = "<operand 1> + <operand 2> — сложение двух чисел"; 

static const plugin_descriptor desc{
    "Addition", "+", 2, &add_apply, help
};

PLUGIN_EXPORT const plugin_descriptor* plugin_get_descriptor() { return &desc; }
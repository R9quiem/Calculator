#include "plugin_api.h"
#include <cmath>

static double PLUGIN_CALL ln_apply(const double* a, unsigned int n) {
    return std::log(a[0]);
}

const char *help = "ln(<operand 1>) — натуральный логарифм <operand 1>."; 

static const plugin_descriptor desc{
    "Natural logarithm", "ln", 1, &ln_apply, help
};

PLUGIN_EXPORT const plugin_descriptor* plugin_get_descriptor() { return &desc; }
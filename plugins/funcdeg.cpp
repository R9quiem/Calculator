#include "plugin_api.h"
#include <cmath>

static double PLUGIN_CALL deg_apply(const double* a, unsigned int n) {
    return std::pow(a[0],a[1]);
}

const char *help = "<operand 1> ^ <operand 2> — возведение <operand 1> в степень <operand 2>."; 

static const plugin_descriptor desc{
    "Exponentiation", "^", 2, &deg_apply, help
};

PLUGIN_EXPORT const plugin_descriptor* plugin_get_descriptor() { return &desc; }
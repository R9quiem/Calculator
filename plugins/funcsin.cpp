#include "plugin_api.h"
#include <cmath>

static double PLUGIN_CALL sin_apply(const double* a, unsigned int n) {
    return std::sin(a[0]);
}

const char *help = "sin(<operand 1>) — взятие синуса от <operand 1>."; 

static const plugin_descriptor desc{
    "Sine", "sin", 1, &sin_apply, help
};

PLUGIN_EXPORT const plugin_descriptor* plugin_get_descriptor() { return &desc; }
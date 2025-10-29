#include "plugin_api.h"
#include <cmath>
#include <stdexcept>

static double PLUGIN_CALL ln_apply(const double* a, unsigned int n) {
    const double x = a[0];
    if (x <= 0.0)
        throw std::domain_error("log: аргумент должен быть > 0");
    return std::log(a[0]);
}

static const char *help = "ln(<operand 1>) — натуральный логарифм <operand 1>."; 

static const plugin_descriptor desc{
    "Natural logarithm", "ln", 1, &ln_apply, help
};

PLUGIN_EXPORT const plugin_descriptor* plugin_get_descriptor() { return &desc; }
#include "plugin_api.h"
#include <cmath>
#include <stdexcept>

static bool is_integer(double x) {
    return std::floor(x) == x;
}

static double PLUGIN_CALL deg_apply(const double* a, unsigned int n) {
    const double base = a[0], exp = a[1];

    if (base == 0.0 && exp <= 0.0) throw std::domain_error("deg: a^b при a=0 и b<=0 не определено");
    if (base < 0.0 && !is_integer(exp)) throw std::runtime_error("pow: отрицательная база с нецелой степенью");
    return std::pow(a[0],a[1]);
}

static const char *help = "<operand 1> ^ <operand 2> — возведение <operand 1> в степень <operand 2>."; 

static const plugin_descriptor desc{
    "Exponentiation", "^", 2, &deg_apply, help, 3, ASSOC_RIGHT
};

PLUGIN_EXPORT const plugin_descriptor* plugin_get_descriptor() { return &desc; }    
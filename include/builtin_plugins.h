#pragma once
#include <stdexcept>

inline double add_fn(const double* args, unsigned int n) { return args[0] + args[1]; }
inline double sub_fn(const double* args, unsigned int n) { return args[0] - args[1]; }
inline double mul_fn(const double* args, unsigned int n) { return args[0] * args[1]; }
inline double div_fn(const double* args, unsigned int n) {
    if (args[1] == 0) throw std::runtime_error("div: деление на 0");   
    return args[0] / args[1]; 
}
static double unary_minus_fn(const double* args, unsigned int n) {
    return -args[0];
}

#pragma once

inline double add_fn(const double* args, unsigned int n) { return args[0] + args[1]; }
inline double sub_fn(const double* args, unsigned int n) { return args[0] - args[1]; }
inline double mul_fn(const double* args, unsigned int n) { return args[0] * args[1]; }
inline double div_fn(const double* args, unsigned int n) { return args[0] / args[1]; }

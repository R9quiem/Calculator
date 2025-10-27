#pragma once
#include <cstddef>

#ifdef _WIN32
  #define PLUGIN_EXPORT extern "C" __declspec(dllexport)
  #define PLUGIN_CALL __cdecl
#else
  #define PLUGIN_EXPORT extern "C"
  #define PLUGIN_CALL
#endif

static unsigned int PLUGIN_ARITY_VARIADIC = 0;

using plugin_apply_fn = double (PLUGIN_CALL *)(const double* args, unsigned int n);

struct plugin_descriptor {
    const char*      name;
    const char*      symbol;
    unsigned int     arity;
    plugin_apply_fn  apply;
    const char*    (*help)();
};

PLUGIN_EXPORT const plugin_descriptor* plugin_get_descriptor();
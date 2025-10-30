#pragma once

#include "PluginRegistry.h"
#include "builtin_plugins.h"
#include <filesystem>

namespace fs = std::filesystem;

void load_builtin_plugins(PluginRegistry& reg);
void load_dll_plugins(PluginRegistry& reg);
void load_plugins(PluginRegistry& reg);


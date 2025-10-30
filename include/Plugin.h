#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>
#include "DllLoader.h"    // требуется тут, т.к. DllLoader — data-member
#include "plugin_api.h"   // для plugin_descriptor и PLUGIN_CALL

// Ассоциативность операторов
enum class Associativity : unsigned int {
    ASSOC_LEFT  = 0,
    ASSOC_RIGHT = 1,
};

struct Plugin {
    // ==== данные плагина ====
    std::filesystem::path path_;
    DllLoader             dll_;      // владелец загруженной DLL (для внешних плагинов)
    std::string           symbol_;
    std::string           name_;
    unsigned int          arity_{};
    std::function<double(const double*, unsigned int)> exec_; // единая точка вызова
    std::string           help_{};
    unsigned int          priority_{};
    Associativity         assoc_{Associativity::ASSOC_LEFT};

    using GetDescFn = const plugin_descriptor* (PLUGIN_CALL *)();

public:
    // ---- конструкторы ----
    // Загрузка плагина из DLL
    explicit Plugin(const std::filesystem::path& dll_path);

    // Встроенный плагин
    using builtin_fn = double (*)(const double*, unsigned int);
    Plugin(std::string symbol, std::string name, unsigned int arity,
           builtin_fn fn, std::string help = "no info",
           Associativity assoc = Associativity::ASSOC_LEFT,
           unsigned int priority = 0);

    Plugin(const Plugin&) = delete;
    Plugin& operator=(const Plugin&) = delete;

    Plugin(Plugin&&) noexcept = default;
    Plugin& operator=(Plugin&&) noexcept = default;

    // ---- доступ к метаданным ----
    const std::string& symbol()         const noexcept { return symbol_; }
    const std::string& name()           const noexcept { return name_; }
    unsigned int       arity()          const noexcept { return arity_; }
    const std::filesystem::path& path() const noexcept { return path_; }
    std::string        help()           const noexcept { return help_; }
    unsigned int       priority()       const noexcept { return priority_; }
    Associativity      assoc()          const noexcept { return assoc_; }

    // ---- вызов операции ----
    double call(const double* args, unsigned int n) const;
    double call(const std::vector<double>& args) const;
};

#pragma once

#include <string>
#include <vector>
#include "plugin_api.h"
#include <filesystem>
#include "DllLoader.h"

struct Plugin {
    std::filesystem::path path_;
    DllLoader             dll_;       // владелец загруженной DLL
    std::string           symbol_;  
    std::string           name_;
    unsigned int          arity_{};
    plugin_apply_fn       apply_{};
    std::string           help_{}; 

    using GetDescFn = const plugin_descriptor* (PLUGIN_CALL *)();

public:

    explicit Plugin(const std::filesystem::path& dll_path)
        : path_(dll_path)
        , dll_(dll_path)
    {
        auto fn = reinterpret_cast<GetDescFn>(dll_.load_symbol_raw("plugin_get_descriptor"));
        const plugin_descriptor* d = fn();
        if (!d || !d->apply || !d->symbol || !*d->symbol)
            throw std::runtime_error("Invalid plugin descriptor: " + dll_path.string());

        symbol_ = d->symbol;
        name_   = d->name ? d->name : "";
        arity_  = d->arity;
        apply_  = d->apply;
        help_   = d->help ? d->help : "no info";
    }
    Plugin(const Plugin&) = delete;
    Plugin& operator=(const Plugin&) = delete;

    //все поля Plugin либо стандартные контейнеры, либо DllLoader, который реализует move-семантику.
    Plugin(Plugin&&) noexcept = default;
    Plugin& operator=(Plugin&&) noexcept = default;

    const std::string& symbol() const noexcept { return symbol_; }
    const std::string& name()   const noexcept { return name_; }
    unsigned int arity()         const noexcept { return arity_; }
    const std::filesystem::path& path() const noexcept { return path_; }
    std::string help() const noexcept { return help_; }

    // Вызов операции
    double call(const double* args, std::size_t n) const {
        if (!apply_) throw std::runtime_error("Плагин не инициализирован");

        if (arity_ != 0 && n != arity_)
            throw std::invalid_argument("Неверное число аргументов");
        return apply_(args, n);
    }

    // перегрузка для std::vector
    double call(const std::vector<double>& args) const {
        return call(args.data(), args.size());
    }

};
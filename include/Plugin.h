#pragma once

#include <string>
#include <vector>
#include "plugin_api.h"
#include <filesystem>
#include "DllLoader.h"
#include <functional>

enum class Associativity : unsigned int {
    ASSOC_LEFT  = 0,
    ASSOC_RIGHT = 1,  
};

struct Plugin {
    std::filesystem::path path_;
    DllLoader             dll_;       // владелец загруженной DLL
    std::string           symbol_;  
    std::string           name_;
    unsigned int          arity_{};
    std::function<double(const double*, unsigned int)> exec_; //единая точка вызова
    std::string           help_{}; 

    unsigned int          priority_;
    Associativity         assoc_;

    using GetDescFn = const plugin_descriptor* (PLUGIN_CALL *)();

public:

    explicit Plugin(const std::filesystem::path& dll_path)
        : path_(dll_path)
        , dll_(dll_path)
    {
        auto fn = reinterpret_cast<GetDescFn>(dll_.load_symbol_raw("plugin_get_descriptor"));
        const plugin_descriptor* d = fn();
        if (!d || !d->apply || !d->symbol || !*d->symbol || !d->arity)
            throw std::runtime_error("Некорректный дескриптор плагина: " + dll_path.string());

        symbol_ = d->symbol;
        name_   = d->name ? d->name : "";
        arity_  = d->arity;
        help_   = d->help ? d->help : "no info";
        priority_ = d->priority;
        assoc_ = static_cast<Associativity>(d->associativity);

        auto fp = d->apply;
        exec_ = [fp](const double* args, unsigned int n) -> double {
            return fp(args,n);
        };
    }
    using builtin_fn = double (*)(const double*, unsigned int);
    Plugin(const std::string symbol, const std::string name, const unsigned int arity,
           builtin_fn fn, std::string help = "no info",
            Associativity assoc = Associativity::ASSOC_LEFT, unsigned int priority = 0
        )
        : symbol_   (symbol),
          name_     (name),
          help_     (help),
          arity_    (arity),
          exec_     (fn),
          assoc_    (assoc),
          priority_ (priority)
    {
        if (symbol_.empty() || !exec_ || !arity_)
            throw std::invalid_argument("Встроенный плагин: нету символа/функции/арности");
    }
    Plugin(const Plugin&) = delete;
    Plugin& operator=(const Plugin&) = delete;

    //все поля Plugin либо стандартные контейнеры, либо DllLoader, который реализует move-семантику.
    Plugin(Plugin&&) noexcept = default;
    Plugin& operator=(Plugin&&) noexcept = default;

    const std::string& symbol()         const noexcept { return symbol_; }
    const std::string& name()           const noexcept { return name_; }
    unsigned int arity()                const noexcept { return arity_; }
    const std::filesystem::path& path() const noexcept { return path_; }
    std::string help()                  const noexcept { return help_; }
    unsigned int priority()             const noexcept { return priority_; }
    Associativity assoc()               const noexcept { return assoc_; }

    // Вызов операции
    double call(const double* args, unsigned int n) const {
        if (!exec_) throw std::runtime_error("Плагин не инициализирован");

        if (arity_ != 0 && n != arity_)
            throw std::invalid_argument("Неверное число аргументов");
        return exec_(args, n);
    }

    // перегрузка для std::vector
    double call(const std::vector<double>& args) const {
        return call(args.data(), args.size());
    }

};
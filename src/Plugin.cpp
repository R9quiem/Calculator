#include "Plugin.h"

#include <stdexcept>
#include <utility>

// ====== Конструктор: загрузка из DLL ======
Plugin::Plugin(const std::filesystem::path& dll_path)
    : path_(dll_path)
    , dll_(dll_path)
{
    auto fn = reinterpret_cast<GetDescFn>(dll_.load_symbol_raw("plugin_get_descriptor"));
    const plugin_descriptor* d = fn();
    if (!d || !d->apply || !d->symbol || !*d->symbol || !d->arity) {
        throw std::runtime_error("Некорректный дескриптор плагина: " + dll_path.string());
    }

    symbol_   = d->symbol;
    name_     = d->name ? d->name : "";
    arity_    = d->arity;
    help_     = d->help ? d->help : "no info";
    priority_ = d->priority;
    assoc_    = static_cast<Associativity>(d->associativity);

    auto fp = d->apply;
    exec_ = [fp](const double* args, unsigned int n) -> double {
        return fp(args, n);
    };
}

// ====== Конструктор: встроенный плагин ======
Plugin::Plugin(std::string symbol, std::string name, unsigned int arity,
               builtin_fn fn, std::string help,
               Associativity assoc, unsigned int priority)
    : symbol_(std::move(symbol))
    , name_(std::move(name))
    , arity_(arity)
    , exec_(fn)
    , help_(std::move(help))
    , priority_(priority)
    , assoc_(assoc)
{
    if (symbol_.empty() || !exec_ || !arity_) {
        throw std::invalid_argument("Встроенный плагин: нету символа/функции/арности");
    }
}

// ====== Вызовы ======
double Plugin::call(const double* args, unsigned int n) const {
    if (!exec_) throw std::runtime_error("Плагин не инициализирован");
    if (arity_ != 0 && n != arity_)
        throw std::invalid_argument("Неверное число аргументов");
    return exec_(args, n);
}

double Plugin::call(const std::vector<double>& args) const {
    return call(args.data(), static_cast<unsigned int>(args.size()));
}

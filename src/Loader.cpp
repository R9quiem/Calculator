#include "Loader.h"

void load_builtin_plugins(PluginRegistry& reg){
    reg.add(std::make_unique<Plugin>("+", "Addition", 2, add_fn, "a + b",
                                     Associativity::ASSOC_LEFT, 1));
    reg.add(std::make_unique<Plugin>("-", "Subtraction", 2, sub_fn, "a - b",
                                     Associativity::ASSOC_LEFT, 1));
    reg.add(std::make_unique<Plugin>("*", "Multiplication", 2, mul_fn, "a * b",
                                     Associativity::ASSOC_LEFT, 2));
    reg.add(std::make_unique<Plugin>("/", "Division", 2, div_fn, "a / b",
                                     Associativity::ASSOC_LEFT, 2));
}

void load_dll_plugins(PluginRegistry& reg){
    const fs::path plugin_dir = fs::current_path() / "plugins";
    if (!fs::exists(plugin_dir)) {
        std::cerr << "Папка с плагинами не найдена: " << plugin_dir << "\n";
        return;
    }
    for (const auto& entry : fs::directory_iterator(plugin_dir)) {
        const fs::path& path = entry.path();
        if (!entry.is_regular_file() || path.extension() != ".dll") continue;
        try {
            auto plugin = std::make_unique<Plugin>(path);
            reg.add(std::move(plugin));
        } catch (const std::exception& ex) {
            std::cerr << "Ошибка при загрузке " << path.filename()
                      << ": " << ex.what() << "\n";
        }
    }
}

void load_plugins(PluginRegistry& reg){
    load_builtin_plugins(reg);
    load_dll_plugins(reg);
}
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>

#include "Plugin.h"

class PluginRegistry {
    std::unordered_map<std::string, std::unique_ptr<Plugin>> plugins;

public:
    const Plugin& at(const std::string& sym) const {
        auto it = plugins.find(sym);
        if (it == plugins.end()) throw std::runtime_error("Unknown op/function: " + sym);
        return *it->second;
    }
    int priority_of(const std::string& sym) const {
        auto it = plugins.find(sym);
        if (it == plugins.end()) throw std::runtime_error("Unknown op/function: " + sym);
        return static_cast<int>(it->second->priority());
    }
    bool is_right_assoc(const std::string& sym) const {
        auto it = plugins.find(sym);
        if (it == plugins.end()) throw std::runtime_error("Unknown op/function: " + sym);
        return it->second->assoc() == Associativity::ASSOC_RIGHT;
    }
    void add(std::unique_ptr<Plugin> p) {
        plugins[p->symbol()] = std::move(p);
        std::cout << "Загружен плагин: " << p->symbol() << " (" << p->name() << ") " << std::endl;
    }
    std::vector<std::string_view> get_all_sym() const {
        std::vector<std::string_view> symbols;
        for(const auto& [sym, plugin] : plugins) {
            symbols.push_back(sym);
        }
        return symbols;
    }
};
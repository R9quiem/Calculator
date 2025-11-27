#pragma once

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <vector>
#include <string_view>
#include <windows.h>
#include <algorithm>

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
        std::string sym = p->symbol(), name = p->name(); 
        plugins[p->symbol()] = std::move(p);
        std::cout << "Загружен плагин: " << sym << " (" << name << ") " << std::endl;
    }
    std::vector<std::string> get_all_sym() const {
        std::vector<std::string> symbols;
        symbols.reserve(plugins.size());
        for (const auto& [sym, plugin] : plugins) {
            symbols.push_back(sym); // копия — string_view не нужен и не «висячий»
        }
        // полезно, если будут многосимвольные токены (например, "==", "<=", "**")
        std::sort(symbols.begin(), symbols.end(),
                [](const std::string& a, const std::string& b){ return a.size() > b.size(); });
        return symbols;
    }
};
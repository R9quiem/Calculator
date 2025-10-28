#pragma once

#include <windows.h>
#include <filesystem>
#include "plugin_api.h"

// Вспомогательная обёртка для сырой DLL.
// загрузка/выгрузка/поиск символа.
class DllLoader {
    HMODULE h_ = nullptr;

    void close() noexcept {
        if (h_) { ::FreeLibrary(h_); h_ = nullptr; }
    }

public:
    DllLoader() = default;
    explicit DllLoader(const std::filesystem::path& path) {
        h_ = ::LoadLibraryW(path.wstring().c_str());
        if (!h_)
            throw std::runtime_error("Не удалось загрузить DLL: " + path.string());
    }

    DllLoader(const DllLoader&) = delete;
    DllLoader& operator=(const DllLoader&) = delete;

    DllLoader(DllLoader&& o) noexcept : h_(o.h_) { o.h_ = nullptr; }
    DllLoader& operator=(DllLoader&& o) noexcept {
        if (this != &o) { close(); h_ = o.h_; o.h_ = nullptr; }
        return *this;
    }

    ~DllLoader() { close(); }

    void* load_symbol_raw(const char* name) const {
        if (!h_) throw std::runtime_error("DLL не загружена");
        void* p = reinterpret_cast<void*>(::GetProcAddress(h_, name));
        if (!p) throw std::runtime_error(std::string("GetProcAddress failed: ") + name);
        return p;
    }
};
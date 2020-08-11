#pragma once

#include <cstdint>

class Static_string {
public:
    template <uint32_t N>
    constexpr Static_string(const char (&str)[N]) : size_{N - 1}, str_{str} {}

    /// Возвращает размер строки (без \0).
    constexpr uint32_t size() const {
        return size_;
    }

    /// Возвращает указатель на строку.
    constexpr const char* c_str() const {
        return str_;
    }

private:
    const uint32_t size_;
    const char*    str_;
};

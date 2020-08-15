#pragma once

#include <cstring>

#include "token_by_char.h"

namespace tokens {

template <size_t current_i, char first_char, char... char_pack>
struct tokens_storage_impl {
    static constexpr typename token_by_char<first_char>::type current{};
    static constexpr tokens_storage_impl<current_i + 1, char_pack...>
        next_tokens{};

    constexpr static const token_base_t& token_by_char(char c) {
        if (current.is_value(c)) {
            return current;
        }

        return next_tokens.token_by_char(c);
    }

    constexpr static const token_base_t& at(size_t i) {
        if (i == current_i) {
            return current;
        }

        return next_tokens.at(i);
    }

    constexpr static size_t size() {
        std::cout << current.get_value() << " " << current_i << " | ";
        return next_tokens.size();
    }
};

template <size_t current_i, char first_char>
struct tokens_storage_impl<current_i, first_char> {
    static constexpr typename token_by_char<first_char>::type current{};
    static constexpr empty_token_t                            empty{};

    constexpr static const token_base_t& token_by_char(char c) {
        if (current.is_value(c)) {
            return current;
        }

        return empty;
    }

    constexpr static const token_base_t& at(size_t i) {
        if (i == current_i) {
            return current;
        }

        return empty;
    }

    constexpr static size_t size() {
        std::cout << current.get_value() << " " << current_i << std::endl;
        return current_i + 1;
    }
};


template <char... char_pack>
struct char_pack_to_token_pack {};

template <const char* c_str, size_t current_str_len, char... char_pack>
struct c_str_to_char_pack {
    using result = typename c_str_to_char_pack<c_str, current_str_len - 1,
                                               c_str[current_str_len - 1],
                                               char_pack...>::result;
};

template <const char* c_str, char... char_pack>
struct c_str_to_char_pack<c_str, 0, char_pack...> {
    using result = tokens_storage_impl<0, char_pack...>;
};

template <const char* c_str>
using tokens_storage =
    typename c_str_to_char_pack<c_str, strlen(c_str)>::result;

}  // namespace tokens
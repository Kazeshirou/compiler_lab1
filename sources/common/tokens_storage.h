#pragma once

#include "token_by_char.h"

namespace tokens {

template <char first_char, char... char_pack>
struct tokens_storage_impl {
    static constexpr typename token_by_char<first_char>::type current{};
    static constexpr tokens_storage_impl<char_pack...>        next_tokens{};

    static constexpr token_base_t& token_by_char(char c) {
        if (current.is_value(c)) {
            return current;
        }

        return next_tokens.token_by_char(c);
    }

    static constexpr token_base_t& next() {
        return next_tokens.current;
    }
};

template <char first_char>
struct tokens_storage_impl<first_char> {
    static constexpr typename token_by_char<first_char>::type current{};
    static constexpr empty_token_t                            empty{};

    static constexpr token_base_t& token_by_char(char c) {
        if (current.is_value(c)) {
            return current;
        }

        return empty;
    }

    static constexpr token_base_t& next() {
        return empty;
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
    using result = tokens_storage_impl<char_pack...>;
};

template <const char* c_str>
using tokens_storage =
    typename c_str_to_char_pack<c_str, sizeof(c_str)>::result;

}  // namespace tokens
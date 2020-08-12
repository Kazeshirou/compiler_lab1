#pragma once

#include "token_by_char.h"

namespace tokens {

template <char first_char, char... char_pack>
struct all_tokens_impl {
    static constexpr typename token_by_char<first_char>::type current{};
    static constexpr all_tokens_impl<char_pack...>            next{};
    static constexpr bool                                     has_next{true};

    static constexpr token_base_t& token_by_char(char c) {
        if (current.is_value(c)) {
            return current;
        }

        return next.token_by_char(c);
    }
};
template <char first_char>
struct all_tokens_impl<first_char> {
    static constexpr typename token_by_char<first_char>::type current{};
    static constexpr empty_token_t                            empty{};
    static constexpr bool                                     has_next{false};

    static constexpr token_base_t& token_by_char(char c) {
        if (current.is_value(c)) {
            return current;
        }

        return empty;
    }
};

template <const char* c_str, size_t current_str_len, char... char_pack>
struct c_str_to_char_pack {
    using result = typename c_str_to_char_pack<c_str, current_str_len - 1,
                                               c_str[current_str_len - 1],
                                               char_pack...>::result;
};

template <const char* c_str, char... char_pack>
struct c_str_to_char_pack<c_str, 0, char_pack...> {
    using result = all_tokens_impl<char_pack...>;
};

template <const char* c_str>
using all_tokens = typename c_str_to_char_pack<c_str, sizeof(c_str)>::result;

constexpr char literals[] = "*+`|()abc";
using all_tokens_instance = all_tokens<literals>;

}  // namespace tokens
#pragma once

#include "static_string.h"
#include "tokens.h"

namespace tokens {

template <char VALUE>
struct token_by_char {
    using type = operand_t<VALUE>;
    static constexpr type token{};
};

template <>
struct token_by_char<'('> {
    using type = open_parenthesis_t;
    static constexpr open_parenthesis_t token{};
};

template <>
struct token_by_char<')'> {
    using type = close_parenthesis_t;
    static constexpr type token{};
};

template <>
struct token_by_char<'*'> {
    using type = operator_star_t;
    static constexpr type token{};
};

template <>
struct token_by_char<'+'> {
    using type = operator_plus_t;
    static constexpr type token{};
};

template <>
struct token_by_char<'`'> {
    using type = operator_concat_t;
    static constexpr type token{};
};

}  // namespace tokens
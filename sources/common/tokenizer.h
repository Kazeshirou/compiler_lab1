#pragma once

#include <cstdint>
#include <cstring>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <type_traits>

namespace tokens {

struct token_base_t {};

using token_value_t = char;

template <token_value_t VALUE>
struct token_t : public token_base_t {
    static constexpr token_value_t value{VALUE};
};

struct operator_base_t {};

template <token_value_t VALUE, uint8_t PRIORITY, bool IS_LEFT,
          uint8_t NUM_OF_OPERANDS>
struct operator_t : public token_t<VALUE>, public operator_base_t {
    static constexpr uint8_t priority{PRIORITY};
    static constexpr bool    is_left{IS_LEFT};
    static constexpr uint8_t num_of_operands{NUM_OF_OPERANDS};
};

struct operator_star_t : public operator_t<'*', 9, true, 1> {};
struct operator_plus_t : public operator_t<'+', 9, true, 1> {};
struct operator_concat_t : public operator_t<'`', 5, true, 2> {};
struct operator_or_t : public operator_t<'|', 1, true, 2> {};

template <token_value_t VALUE>
struct operand_t : public token_t<VALUE> {};

struct open_parenthesis_t : public token_t<'('> {};
struct close_parenthesis_t : public token_t<')'> {};

template <class Token>
struct is_operator {
    static constexpr bool value{std::is_base_of<operator_base_t, Token>::value};
};

template <char VALUE>
struct token_by_char {
    using type = operand_t<VALUE>;
};

template <>
struct token_by_char<'('> {
    using type = open_parenthesis_t;
};

template <>
struct token_by_char<')'> {
    using type = close_parenthesis_t;
};

template <>
struct token_by_char<'*'> {
    using type = operator_star_t;
};

template <>
struct token_by_char<'+'> {
    using type = operator_plus_t;
};

template <>
struct token_by_char<'`'> {
    using type = operator_concat_t;
};

template <>
struct token_by_char<'|'> {
    using type = operator_or_t;
};

}  // namespace tokens
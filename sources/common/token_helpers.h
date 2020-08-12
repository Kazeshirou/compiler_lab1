#pragma once

#include <type_traits>

#include "tokens.h"

namespace tokens {

template <class Token>
struct is_operator {
    static constexpr bool value{std::is_base_of<operator_base_t, Token>::value};
};

template <class Token>
struct is_open_parenthesis {
    static constexpr bool value{std::is_same<open_parenthesis_t, Token>::value};
};

template <class Token>
struct is_close_parenthesis {
    static constexpr bool value{
        std::is_same<close_parenthesis_t, Token>::value};
};

}  // namespace tokens
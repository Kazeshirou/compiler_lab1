#include <gtest/gtest.h>

#include "tokenizer.h"

#include <iostream>
#include <vector>

namespace lab1::tests::tokens {

using namespace ::tokens;

TEST(Tokenizer_tests, is_operator) {
    EXPECT_TRUE(is_operator<operator_concat_t>::value);
    EXPECT_FALSE(is_operator<operand_t<'a'>>::value);
}

TEST(Tokenizer_tests, token_by_char) {
    EXPECT_TRUE(
        (std::is_same<open_parenthesis_t, token_by_char<'('>::type>::value));
    EXPECT_FALSE(
        (std::is_same<open_parenthesis_t, token_by_char<')'>::type>::value));

    std::string                s{"*"};
    std::vector<token_base_t*> tokens;
    for (const auto c : s) {
        switch (c) {
            case '*': {
                tokens.push_back(new token_by_char<'*'>::type);
                break;
            }
        }
    }
    for (auto& v : tokens) {
        delete v;
    }
}

}  // namespace lab1::tests::tokens
#include <gtest/gtest.h>

#include "tokenizer.h"

#include <iostream>
#include <vector>

namespace lab1::tests::tokens {

using namespace ::tokens;

TEST(Tokenizer_tests, is_operator) {
    EXPECT_TRUE(is_operator<operator_star_t>::value);
    EXPECT_FALSE(is_operator<operand_t<'a'>>::value);
}

TEST(Tokenizer_tests, token_by_char) {
    EXPECT_TRUE(
        (std::is_same<open_parenthesis_t, token_by_char<'('>::type>::value));
    EXPECT_FALSE(
        (std::is_same<open_parenthesis_t, token_by_char<')'>::type>::value));
}


constexpr char str[] = "(a|b)`c+*";

TEST(Tokenizer_tests, tokens_storage_at) {
    using token_chunk_t = tokens_storage<str>;
    token_chunk_t token_chunk;

    EXPECT_EQ(token_chunk.size(), sizeof(str) - 1);
    for (size_t i = 0; i < token_chunk.size(); i++) {
        EXPECT_EQ(token_chunk.at(i).get_value(), str[i]);
    }
}

TEST(Tokenizer_tests, tokens_storage_token_by_char) {
    using token_chunk_t = tokens_storage<str>;
    token_chunk_t token_chunk;

    EXPECT_TRUE(token_chunk.token_by_char('(').is_open_parenthesis());
    EXPECT_FALSE(token_chunk.token_by_char('(').is_close_parenthesis());
    EXPECT_TRUE(token_chunk.token_by_char('|').is_operator());
    EXPECT_TRUE(token_chunk.token_by_char('`').is_operator());
    EXPECT_TRUE(token_chunk.token_by_char('*').is_operator());
    EXPECT_TRUE(token_chunk.token_by_char('+').is_operator());
    char c = '`';
    EXPECT_EQ(static_cast<const operator_base_t&>(token_chunk.token_by_char(c))
                  .get_priority(),
              operator_concat_t::priority);
}

}  // namespace lab1::tests::tokens
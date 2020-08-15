#include <gtest/gtest.h>

#include "token_helpers.h"
#include "tokenizer.h"

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

TEST(Tokenizer_tests, tokens_storage_at) {
    all_tokens_t tokens;

    EXPECT_EQ(tokens.size(), sizeof(all_literals) - 1);
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens.at(i).get_value(), all_literals[i]);
    }
}

TEST(Tokenizer_tests, tokens_storage_token_by_char) {
    all_tokens_t tokens;

    EXPECT_TRUE(tokens.token_by_char('(').is_open_parenthesis());
    EXPECT_FALSE(tokens.token_by_char('(').is_close_parenthesis());
    EXPECT_TRUE(tokens.token_by_char('|').is_operator());
    EXPECT_TRUE(tokens.token_by_char('`').is_operator());
    EXPECT_TRUE(tokens.token_by_char('*').is_operator());
    EXPECT_TRUE(tokens.token_by_char('+').is_operator());
    EXPECT_FALSE(tokens.token_by_char('+').is_operand());
    EXPECT_TRUE(tokens.token_by_char('a').is_operand());
    EXPECT_FALSE(tokens.token_by_char('(').is_operator());
    EXPECT_FALSE(tokens.token_by_char('(').is_operand());
    char c = '`';
    EXPECT_EQ(static_cast<const operator_base_t&>(tokens.token_by_char(c))
                  .get_priority(),
              operator_concat_t::priority);
}

TEST(Tokenizer_tests, tokenize_empty) {
    Tokenizer tokenizer;

    std::string regexp{};

    auto res = tokenizer.tokenize(regexp);
    ASSERT_TRUE(res);
    EXPECT_EQ(res.value().size(), 0);
}

TEST(Tokenizer_tests, tokenize_incorrect) {
    Tokenizer tokenizer;

    std::string regexp{"(ab|"};
    EXPECT_FALSE(tokenizer.tokenize(regexp));

    regexp = "ab|)";
    EXPECT_FALSE(tokenizer.tokenize(regexp));
}

TEST(Tokenizer_tests, tokenize_simple) {
    Tokenizer tokenizer;

    std::string                         regexp{"(a|b)c"};
    static constexpr char               expected_tokens_c[] = "ab|c`";
    tokens_storage_t<expected_tokens_c> expected_tokens;
    size_t                              expected_size = expected_tokens.size();

    auto res = tokenizer.tokenize(regexp);
    ASSERT_TRUE(res);
    auto tokens = res.value();
    EXPECT_EQ(tokens.size(), expected_size);
    for (size_t i = 0; i < expected_size; i++) {
        EXPECT_EQ(tokens.front().get().get_value(),
                  expected_tokens.at(i).get_value());
        tokens.pop();
    }
}

TEST(Tokenizer_tests, tokenize) {
    Tokenizer tokenizer;

    std::string                         regexp1{"a+|bc*(b|c)"};
    std::string                         regexp2{"a+|((b(c*))(b|c))"};
    static constexpr char               expected_tokens_c[] = "a+bc*`bc|`|";
    tokens_storage_t<expected_tokens_c> expected_tokens;
    size_t                              expected_size = expected_tokens.size();

    auto res = tokenizer.tokenize(regexp1);
    ASSERT_TRUE(res);
    auto tokens = res.value();
    ASSERT_EQ(tokens.size(), expected_size);
    for (size_t i = 0; i < expected_size; i++) {
        EXPECT_EQ(tokens.front().get().get_value(),
                  expected_tokens.at(i).get_value());
        tokens.pop();
    }

    res = tokenizer.tokenize(regexp2);
    ASSERT_TRUE(res);
    tokens = res.value();
    ASSERT_EQ(tokens.size(), expected_size);
    for (size_t i = 0; i < expected_size; i++) {
        EXPECT_EQ(tokens.front().get().get_value(),
                  expected_tokens.at(i).get_value());
        tokens.pop();
    }
}


}  // namespace lab1::tests::tokens
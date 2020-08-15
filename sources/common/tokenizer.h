#pragma once

#include <cassert>
#include <functional>
#include <optional>
#include <queue>
#include <stack>
#include <string_view>

#include "all_tokens.h"

namespace tokens {

class Tokenizer {
public:
    using token_t = std::reference_wrapper<const token_base_t>;
    static std::optional<std::queue<token_t>> tokenize(
        const std::string_view& regexp) {
        static constexpr all_tokens_t all_tokens_instance{};

        std::queue<token_t> tokens_in_rpn;
        if (!regexp.size()) {
            return tokens_in_rpn;
        }

        std::stack<token_t> help_stack;
        auto                insert_next_token = [&tokens_in_rpn,
                                  &help_stack](token_t token) -> bool {
            if (token.get().is_operand()) {
                tokens_in_rpn.push(token);
                return true;
            }
            if (token.get().is_operator()) {
                const operator_base_t& cur_operator =
                    static_cast<const operator_base_t&>(token.get());
                auto cur_operator_priority = cur_operator.get_priority();
                while (true) {
                    if (help_stack.empty()) {
                        help_stack.push(token);
                        return true;
                    }

                    const token_base_t& stack_top = help_stack.top().get();
                    if (!stack_top.is_operator()) {
                        help_stack.push(token);
                        return true;
                    }
                    const operator_base_t& stack_top_operator =
                        static_cast<const operator_base_t&>(stack_top);
                    auto stack_top_operator_priority =
                        stack_top_operator.get_priority();
                    if ((stack_top_operator_priority < cur_operator_priority) ||
                        ((stack_top_operator_priority ==
                          cur_operator_priority) &&
                         (!stack_top_operator.get_is_left()))) {
                        help_stack.push(token);
                        return true;
                    }

                    tokens_in_rpn.push(help_stack.top());
                    help_stack.pop();
                }
            }
            if (token.get().is_open_parenthesis()) {
                help_stack.push(token);
                return true;
            }
            if (token.get().is_close_parenthesis()) {
                while (true) {
                    if (help_stack.empty()) {
                        return false;
                    }

                    const token_base_t& stack_top = help_stack.top().get();
                    if (stack_top.is_open_parenthesis()) {
                        help_stack.pop();
                        return true;
                    }

                    tokens_in_rpn.push(help_stack.top());
                    help_stack.pop();
                }
            }
            assert("to rpn error");
            return false;
        };

        auto finalize_help_stack = [&tokens_in_rpn, &help_stack]() -> bool {
            while (true) {
                if (help_stack.empty()) {
                    return true;
                }

                const token_base_t& stack_top = help_stack.top().get();
                if (stack_top.is_open_parenthesis()) {
                    return false;
                }

                tokens_in_rpn.push(help_stack.top());
                help_stack.pop();
            }
        };

        auto is_unary_operator = [](token_t token) -> bool {
            return token.get().is_operator() &&
                   (static_cast<const operator_base_t&>(token.get())
                        .get_num_of_operands() == 1);
        };

        auto need_concat = [&is_unary_operator](token_t cur_token,
                                                token_t next_token) -> bool {
            return (cur_token.get().is_operand() ||
                    cur_token.get().is_close_parenthesis() ||
                    is_unary_operator(cur_token)) &&
                   (next_token.get().is_operand() ||
                    next_token.get().is_open_parenthesis());
        };


        size_t  regexp_size = regexp.size() - 1;
        token_t cur_token   = all_tokens_instance.token_by_char(regexp[0]);
        token_t next_token  = cur_token;
        for (size_t i = 0; i < regexp_size; i++) {
            cur_token  = next_token;
            next_token = all_tokens_instance.token_by_char(regexp[i + 1]);
            if (!insert_next_token(cur_token)) {
                return {};
            }
            if (need_concat(cur_token, next_token)) {
                if (!insert_next_token(all_tokens_instance.token_by_char(
                        operator_concat_t{}.get_value()))) {
                    return {};
                }
            }
        }
        if (!insert_next_token(next_token)) {
            return {};
        }
        if (!finalize_help_stack()) {
            return {};
        }

        return tokens_in_rpn;
    }
};

}  // namespace tokens
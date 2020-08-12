#pragma once

#include <cstdint>

namespace tokens {

using token_value_t = char;

struct token_base_t {
    virtual token_value_t get_value() const                              = 0;
    virtual bool          is_value(const token_value_t test_value) const = 0;
    virtual bool          is_operator() const                            = 0;
    virtual bool          is_open_parenthesis() const                    = 0;
    virtual bool          is_close_parenthesis() const                   = 0;
    virtual bool          is_empty() const                               = 0;
};

template <token_value_t VALUE>
struct token_t : public token_base_t {
    bool is_value(const token_value_t test_value) const override {
        return test_value == VALUE;
    }

    template <token_value_t TEST_VALUE>
    struct is_value_t {
        static constexpr bool value{TEST_VALUE == VALUE};
    };

    token_value_t get_value() const override {
        return value;
    }

    bool is_operator() const override {
        return false;
    }

    bool is_open_parenthesis() const override {
        return false;
    }

    bool is_close_parenthesis() const override {
        return false;
    }

    bool is_empty() const override {
        return false;
    }

    static constexpr token_value_t value{VALUE};
};

struct empty_token_t : public token_t<'\0'> {
    bool is_empty() const override {
        return true;
    }
};

struct operator_base_t : public token_base_t {
    virtual uint8_t get_priority() const        = 0;
    virtual bool    get_is_left() const         = 0;
    virtual uint8_t get_num_of_operands() const = 0;
};

template <token_value_t VALUE, uint8_t PRIORITY, bool IS_LEFT,
          uint8_t NUM_OF_OPERANDS>
struct operator_t : public operator_base_t {
    bool is_value(const token_value_t test_value) const override {
        return test_value == VALUE;
    }

    template <token_value_t TEST_VALUE>
    struct is_value_t {
        static constexpr bool value{TEST_VALUE == VALUE};
    };

    token_value_t get_value() const override {
        return value;
    }

    bool is_operator() const override {
        return true;
    }

    bool is_open_parenthesis() const override {
        return false;
    }

    bool is_close_parenthesis() const override {
        return false;
    }

    bool is_empty() const override {
        return false;
    }

    uint8_t get_priority() const override {
        return priority;
    }

    bool get_is_left() const override {
        return is_left;
    }

    uint8_t get_num_of_operands() const override {
        return num_of_operands;
    }


    static constexpr uint8_t priority{PRIORITY};
    static constexpr bool    is_left{IS_LEFT};
    static constexpr uint8_t num_of_operands{NUM_OF_OPERANDS};

    static constexpr token_value_t value{VALUE};
};

struct operator_star_t : public operator_t<'*', 9, true, 1> {};

struct operator_plus_t : public operator_t<'+', 9, true, 1> {};

struct operator_concat_t : public operator_t<'`', 5, true, 2> {};

struct operator_or_t : public operator_t<'|', 1, true, 2> {};

template <token_value_t VALUE>
struct operand_t : public token_t<VALUE> {};

struct open_parenthesis_t : public token_t<'('> {
    bool is_open_parenthesis() const override {
        return true;
    }
};
struct close_parenthesis_t : public token_t<')'> {
    bool is_close_parenthesis() const override {
        return true;
    }
};

}  // namespace tokens
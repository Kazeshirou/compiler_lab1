#pragma once

#include "tokens_storage.h"
namespace tokens {

constexpr char all_literals[] = "*+`|()abc";
using all_tokens_instance     = tokens_storage<all_literals>;

}  // namespace tokens

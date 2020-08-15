#pragma once

#include "tokens_storage.h"
namespace tokens {

constexpr char all_literals[] = "*+`|()abc";
using all_tokens_t            = tokens_storage_t<all_literals>;

}  // namespace tokens

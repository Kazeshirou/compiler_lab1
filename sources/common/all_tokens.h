#pragma once

#include "tokens_storage.h"
namespace tokens {

constexpr char literals[] = "*+`|()abc";
using all_tokens_instance = tokens_storage<literals>;

}  // namespace tokens

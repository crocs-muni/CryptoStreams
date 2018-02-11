#pragma once

#include <cstdint>
#include <streams/hash/hash_interface.h>

namespace sha3 {

using BitSequence = unsigned char;
using DataLength = unsigned long long;

struct sha3_interface : hash::hash_interface{
};

} // namespace sha3

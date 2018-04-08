#pragma once

#include <string>
#include <memory>
#include "block_cipher.h"

#include <algorithm>

#include "ciphers/tea/tea.h"
#include "ciphers/aes/aes.h"
#include "ciphers/aria/aria_block.h"
#include "ciphers/camellia/camellia_block.h"
#include "ciphers/rc4/rc4.h"
#include "ciphers/simon/simon.h"
#include "ciphers/speck/speck.h"
#include "ciphers/des/single_des.h"
#include "ciphers/des/triple_des.h"
#include "ciphers/blowfish/blowfish_factory.h"
#include "ciphers/gost/gost_block.h"
#include "ciphers/seed/seed_block.h"
#include "ciphers/idea/idea_block.h"
#include "ciphers/cast/cast_block.h"
#include <streams/block/ciphers/kasumi/kasumi_factory.h>
#include <streams/block/ciphers/kuznyechik/kuznyechik_factory.h>
#include <streams/block/ciphers/misty1/misty1_factory.h>
#include <streams/block/ciphers/noekeon/noekeon_factory.h>

namespace block {
    std::unique_ptr<block_cipher> make_block_cipher(const std::string& name, const std::size_t round, const std::size_t block_size, const std::size_t key_size, const bool encrypt);
}

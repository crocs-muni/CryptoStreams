#pragma once

#include <memory>
#include "block_cipher.h"

#include <algorithm>
#include <eacirc-core/memory.h>

#include "ciphers/tea/tea.h"
#include "ciphers/aes/aes.h"
#include "ciphers/rc4/rc4.h"
#include "ciphers/des/single_des.h"
#include "ciphers/des/triple_des.h"
#include "ciphers/blowfish/blowfish_factory.h"


namespace block {
    std::unique_ptr<block_cipher> make_block_cipher(const std::string& name, unsigned round, unsigned block_size, bool encrypt);
}

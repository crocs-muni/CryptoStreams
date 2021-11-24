#pragma once

#include "block_cipher.h"
#include <algorithm>
#include <memory>
#include <string>
#include <stdexcept>
#include <eacirc-core/json.h>

#include "ciphers/aes/aes.h"
#include "ciphers/aria/aria_block.h"
#include "ciphers/blowfish/blowfish_factory.h"
#include "ciphers/camellia/camellia_block.h"
#include "ciphers/cast/cast_block.h"
#include "ciphers/des/single_des.h"
#include "ciphers/des/triple_des.h"
#include "ciphers/gost/gost_block.h"
#include "ciphers/idea/idea_block.h"
#include "ciphers/kasumi/kasumi_factory.h"
#include "ciphers/kuznyechik/kuznyechik_factory.h"
#include "ciphers/mars/mars.h"
#include "ciphers/misty1/misty1_factory.h"
#include "ciphers/noekeon/noekeon_factory.h"
#include "ciphers/rc6/rc6.h"
#include "ciphers/seed/seed_block.h"
#include "ciphers/serpent/serpent.h"
#include "ciphers/shacal2/shacal2_factory.h"
#include "ciphers/simon/simon.h"
#include "ciphers/speck/speck.h"
#include "ciphers/tea/tea.h"
#include "ciphers/twofish/twofish.h"
#include "ciphers/xtea/xtea_factory.h"
#include "ciphers/lightweight/chaskey/chaskey.h"
#include "ciphers/lightweight/fantomas/fantomas.h"
#include "ciphers/lightweight/hight/hight.h"
#include "ciphers/lightweight/lblock/lblock.h"
#include "ciphers/lightweight/lea/lea.h"
#include "ciphers/lightweight/led/led.h"
#include "ciphers/lightweight/piccolo/piccolo.h"
#include "ciphers/lightweight/pride/pride.h"
#include "ciphers/lightweight/prince/prince.h"
#include "ciphers/lightweight/rc5-20/rc5_20.h"
#include "ciphers/lightweight/rectangle/rectangle.h"
#include "ciphers/lightweight/road_runner/road_runner.h"
#include "ciphers/lightweight/robin/robin.h"
#include "ciphers/lightweight/robin_star/robin_star.h"
#include "ciphers/lightweight/sparx/sparx.h"
#include "ciphers/lightweight/twine/twine.h"
#include "ciphers/lowmc/lowmc.h"

namespace block {
std::unique_ptr<block_cipher> make_block_cipher(const std::string &name,
                                                const std::size_t round,
                                                const std::size_t block_size = 0,
                                                const std::size_t key_size = 0,
                                                const bool encrypt = true,
                                                const nlohmann::json * config = nullptr);
}

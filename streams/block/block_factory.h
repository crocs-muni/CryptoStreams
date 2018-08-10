#pragma once

#include "block_cipher.h"
#include <algorithm>
#include <memory>
#include <string>

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
#include <streams/block/ciphers/lightweight/chaskey/chaskey.h>
#include <streams/block/ciphers/lightweight/fantomas/fantomas.h>
#include <streams/block/ciphers/lightweight/hight/hight.h>
#include <streams/block/ciphers/lightweight/lblock/lblock.h>
#include <streams/block/ciphers/lightweight/lea/lea.h>
#include <streams/block/ciphers/lightweight/led/led.h>
#include <streams/block/ciphers/lightweight/piccolo/piccolo.h>
#include <streams/block/ciphers/lightweight/pride/pride.h>
#include <streams/block/ciphers/lightweight/prince/prince.h>
#include <streams/block/ciphers/lightweight/rc5-20/rc5_20.h>
#include <streams/block/ciphers/lightweight/rectangle/rectangle.h>
#include <streams/block/ciphers/lightweight/road_runner/road_runner.h>
#include <streams/block/ciphers/lightweight/robin/robin.h>
#include <streams/block/ciphers/lightweight/robin_star/robin_star.h>
#include <streams/block/ciphers/lightweight/sparx/sparx.h>
#include <streams/block/ciphers/lightweight/twine/twine.h>

namespace block {
std::unique_ptr<block_cipher> make_block_cipher(const std::string &name,
                                                const std::size_t round,
                                                const std::size_t block_size,
                                                const std::size_t key_size,
                                                const bool encrypt);
}

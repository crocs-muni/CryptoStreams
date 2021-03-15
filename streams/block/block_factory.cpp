#include "block_factory.h"

namespace block {

struct block_cipher;

std::unique_ptr<block_cipher> make_block_cipher(const std::string &name,
                                                const std::size_t round,
                                                const std::size_t block_size,
                                                const std::size_t key_size,
                                                const bool encrypt,
                                                const nlohmann::json * config) {
    // clang-format off
    if (name == "TEA")  return std::make_unique<tea>(round);
    if (name == "AES")  return std::make_unique<aes>(round);
    if (name == "ARIA")  return std::make_unique<aria::aria>(round, encrypt);
    if (name == "CAMELLIA")  return std::make_unique<camellia::camellia>(round, encrypt);
    if (name == "CAST")  return std::make_unique<cast::cast>(round);
    if (name == "IDEA")  return std::make_unique<idea::idea>(round, encrypt);
    if (name == "GOST")  return std::make_unique<gost>(round);
    if (name == "SIMON")  return std::make_unique<simon>(round, block_size, key_size);
    if (name == "SPECK")  return std::make_unique<speck>(round, block_size, key_size);
    if (name == "SINGLE-DES")  return std::make_unique<single_des>(round, encrypt);
    if (name == "TRIPLE-DES")  return std::make_unique<triple_des>(round, encrypt);
    if (name == "BLOWFISH") return std::make_unique<blowfish_factory>(round);
    if (name == "MARS") return std::make_unique<mars::mars>(round, encrypt);
    if (name == "RC6") return std::make_unique<rc6::rc6>(round, encrypt);
    if (name == "SERPENT") return std::make_unique<serpent::serpent>(round, encrypt);
    if (name == "TWOFISH") return std::make_unique<twofish::twofish>(round);
    if (name == "SEED") return std::make_unique<seed::seed>(round);
    if (name == "KASUMI") return std::make_unique<kasumi_factory>(round);
    if (name == "KUZNYECHIK") return std::make_unique<kuznyechik_factory>(round);
    if (name == "MISTY1") return std::make_unique<misty1_factory>(round);
    if (name == "NOEKEON") return std::make_unique<noekeon_factory>(round);
    if (name == "SHACAL2") return std::make_unique<shacal2_factory>(round);
    if (name == "XTEA") return std::make_unique<xtea_factory>(round);
    if (name == "CHASKEY") return std::make_unique<chaskey>(round);
    if (name == "FANTOMAS") return std::make_unique<fantomas>(round);
    if (name == "HIGHT") return std::make_unique<hight>(round);
    if (name == "LBLOCK") return std::make_unique<lblock>(round);
    if (name == "LEA") return std::make_unique<lea>(round);
    if (name == "LED") return std::make_unique<led>(round);
    if (name == "PICCOLO") return std::make_unique<piccolo>(round);
    if (name == "PRIDE") return std::make_unique<pride>(round, encrypt);
    if (name == "PRINCE") return std::make_unique<prince>(round);
    if (name == "RC5-20") return std::make_unique<rc5_20>(round);
    if (name == "RECTANGLE-K80") return std::make_unique<rectangle_k80>(round);
    if (name == "RECTANGLE-K128") return std::make_unique<rectangle_k128>(round);
    if (name == "ROAD-RUNNER-K80") return std::make_unique<road_runner_k80>(round, encrypt);
    if (name == "ROAD-RUNNER-K128") return std::make_unique<road_runner_k128>(round, encrypt);
    if (name == "ROBIN") return std::make_unique<robin>(round);
    if (name == "ROBIN-STAR") return std::make_unique<robin_star>(round);
    if (name == "SPARX-B64") return std::make_unique<sparx_b64>(round);
    if (name == "SPARX-B128") return std::make_unique<sparx_b128>(round);
    if (name == "TWINE") return std::make_unique<twine>(round);
    if (name == "LOWMC") return std::make_unique<lowmc::lowmc>(round, encrypt, block_size, key_size, config);
    // clang-format on

    throw std::runtime_error("requested block cipher named \"" + name +
                             "\" is either broken or does not exists");
}

} // namespace block

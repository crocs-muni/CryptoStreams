#pragma once

/**
 * Source: nist AES competition submission, downloadable now only from:
 *          https://www.schneier.com/books/applied_cryptography/source.html
 */

#include "../../block_cipher.h"
#include "../common_fun.h"

#include "lowmc_ref.h"
#include <algorithm>
#include <functional>
#include <utility>
#include <eacirc-core/json.h>

namespace block {
namespace lowmc {

/**
 * https://lowmc.github.io/fewmul/talks/tiessen.pdf
 * AES-like security parameters
 *
 * blocksize    sboxes  keysize     data    rounds
 * 256          49      80          64      12
 * 128          31      80          64      12
 * 1024         20      80          64      45
 * 1024         10      80          64      85
 *
 * 256          63      128         128     14
 * 196          63      128         128     14
 * 128           3      128         128     88
 * 128           2      128         128     128
 * 128           1      128         128     252
 * 1024         20      128         128     49
 * 1024         10      128         128     52
 *
 * 512          66      256         256     18
 * 256          10      256         256     52
 * 256           1      256         256     458
 * 1024         10      256         256     103
 *
 * Picnic3 parameters
 * https://microsoft.github.io/Picnic/
 * https://github.com/microsoft/Picnic/blob/master/lowmc_constants.c
 * https://github.com/microsoft/Picnic/blob/master/spec/spec-v3.0.pdf
 *
 * Partial S-box
 * 128          10      128         128     20
 * 192          10      192         192     30
 * 256          10      256         256     38
 *
 * Full S-box
 * 128          43      128         128     4
 * 192          64      192         192     4
 * 255          85      255         255     4
 */

#ifdef BUILD_testsuite
#define LOWMC_CACHE 1
#pragma message "LowMC compiled with cache"
#endif

typedef struct lowmc_paramset_t {
    unsigned bl;
    unsigned sb;
    unsigned ky;
    unsigned dt;
    unsigned r;
} lowmc_paramset_t;

class lowmc : public block_cipher {
private:
    LowMCBase * instance;
    bool flag_cached;

#ifdef LOWMC_CACHE
    static LowMCBase * last_instance;
#endif

public:
    explicit lowmc(std::size_t rounds, bool encrypt = true, std::size_t block_size_bytes = 32, std::size_t key_size_bytes = 10, const json * config = nullptr)
        : block_cipher(rounds), instance(nullptr), flag_cached(false) {

        (void)encrypt;
        std::size_t block_size = block_size_bytes << 3;
        std::size_t key_size = key_size_bytes << 3;

        if (config != nullptr && config->contains("block_size_bits")){
            block_size = config->at("block_size_bits");
        }

        if (config != nullptr && config->contains("key_size_bits")){
            key_size = config->at("key_size_bits");
        }

        unsigned sboxes = 0; // 0 is invalid value - keep default
        if(config != nullptr && config->contains("sboxes")){
            sboxes = config->at("sboxes");
        } else {
            sboxes = lowmc::get_default_sbox_size(block_size, key_size);
        }

        if (sboxes == 0)
            throw std::runtime_error("Sboxes not configured");
        if (block_size == 0)
            throw std::runtime_error("block_size not configured");
        if (key_size == 0)
            throw std::runtime_error("key_size not configured");

#ifdef LOWMC_CACHE
        if (last_instance != nullptr
            && last_instance->getBlocksize() == block_size
            && last_instance->getKeysize() == key_size
            && last_instance->getRounds() == rounds
            && last_instance->getNumofboxes() == sboxes) {
            instance = last_instance;
            flag_cached = true;
        } else {
#endif
            // If no cache is enabled, always create a new instance with new constants generation.
            instance = build_instance(block_size, key_size, rounds, sboxes);
#ifdef LOWMC_CACHE
            last_instance = instance;
            flag_cached = true;
        }
#endif
    }

    ~lowmc() override {
        if (!flag_cached && instance != nullptr){
            delete instance;
            instance = nullptr;
        }
    }

    static void clear_cache(){
#ifdef LOWMC_CACHE
        if (last_instance != nullptr){
            delete last_instance;
            last_instance = nullptr;
        }
#endif
    }

    static LowMCBase * build_instance(std::size_t block_size, std::size_t key_size, std::size_t rounds, std::size_t sboxes){
        if (block_size == 256 && key_size == 80) {
            return new LowMC<256, 80>(rounds, block_size, key_size, sboxes);

        } else if (block_size == 128 && key_size == 80) {
            return new LowMC<128, 80>(rounds, block_size, key_size, sboxes);

        } else if (block_size == 128 && key_size == 128) {
            return new LowMC<128, 128>(rounds, block_size, key_size, sboxes);

        } else if (block_size == 256 && key_size == 128) {
            return new LowMC<256, 128>(rounds, block_size, key_size, sboxes);

        } else if (block_size == 256 && key_size == 256) {
            return new LowMC<256, 256>(rounds, block_size, key_size, sboxes);

        } else if (block_size == 1024 && key_size == 80) {
            return new LowMC<1024, 80>(rounds, block_size, key_size, sboxes);

        } else if (block_size == 1024 && key_size == 128) {
            return new LowMC<1024, 128>(rounds, block_size, key_size, sboxes);

        } else {
            return new LowMC<1024, 256>(rounds, block_size, key_size, sboxes);
        }
    }

    void keysetup(const std::uint8_t* key, const std::uint64_t keysize) override {
        instance->keysetup(key, keysize);
    }

    void ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) override {
        (void)iv;
        (void)ivsize;
        throw std::runtime_error("IV not supported by LowMC");
    }

    void encrypt(const std::uint8_t *plaintext, std::uint8_t *ciphertext) override {
        instance->encrypt(plaintext, ciphertext);
    }

    void decrypt(const std::uint8_t* ciphertext,std::uint8_t* plaintext) override {
        instance->decrypt(ciphertext, plaintext);
    }

private:
    static constexpr const lowmc_paramset_t recom_params[] = {
        {256, 49, 80, 64, 12},
        {128, 31, 80, 64, 12},
        {1024, 20, 80, 64, 45},
        {1024, 10, 80, 64, 85},

        {256, 63, 128, 128, 14},
        {196, 63, 128, 128, 14},
        {128, 3, 128, 128, 88},
        {128, 2, 128, 128, 128},
        {128, 1, 128, 128, 252},
        {1024, 20, 128, 128, 49},
        {1024, 10, 128, 128, 52},

        {512, 66, 256, 256, 18},
        {256, 10, 256, 256, 52},
        {256, 1, 256, 256, 458},
        {1024, 10, 256, 256, 103},

        {128, 10, 128, 128, 20},
        {192, 10, 192, 192, 30},
        {256, 10, 256, 256, 38},

        {128, 43, 128, 128, 4},
        {192, 64, 192, 192, 4},
        {255, 85, 255, 255, 4}
    };

    static const lowmc_paramset_t * get_default_params_by(const std::function<bool(const lowmc_paramset_t &x)>& predicate) {
        const int n = sizeof(recom_params)/sizeof(recom_params[0]);
        auto it = std::find_if(recom_params, recom_params + n, predicate);
        return it == recom_params + n ? nullptr : &(*it);
    }

    static unsigned get_default_sbox_size(unsigned blocksize, unsigned keysize) {
        auto param = get_default_params_by([=](const lowmc_paramset_t &x){return x.bl == blocksize && x.ky == keysize;});
        return param == nullptr ? 0 : param->sb;
    }

    static unsigned get_default_rounds(unsigned blocksize, unsigned keysize) {
        auto param = get_default_params_by([&](const lowmc_paramset_t &x){return x.bl == blocksize && x.ky == keysize;});
        return param == nullptr ? 0 : param->r;
    }
};
} // namespace mars
} // namespace block

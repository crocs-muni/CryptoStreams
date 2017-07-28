#include <gtest/gtest.h>
#include <eacirc-core/external/pcg-cpp-0.98/pcg/pcg_random.hpp>
#include <eacirc-core/random.h>
#include <streams.h>
#include "estream_test_case.h"

namespace testsuite {
    const json estream_test_case::base_config = {
            {"type",           "estream"},
            {"init-frequency", "only-once"},
            {"generator",      "pcg32"},
            {"plaintext-type", {{"type", "test-stream"}}},
            {"iv-type",        {{"type", "test-stream"}}},
            {"key-type",       {{"type", "test-stream"}}},
    };

    void estream_test_case::test(std::unique_ptr<estream_interface> &encryptor) const {
        unsigned char cipher[_plaintext.size()];


        for (int i = 0; i < _plaintext.size(); i++) {
            cipher[i] = 0;
        }

        encryptor->ECRYPT_keysetup((unsigned char *) _key.data(), _key.size() * 8, _iv.size() * 8);
        encryptor->ECRYPT_ivsetup((const u8 *) _iv.data());
        encryptor->ECRYPT_encrypt_bytes((const u8 *) _plaintext.data(), cipher, _plaintext.size());

        std::stringstream ss;

        for (auto byte : cipher) {
            ss << std::setfill('0') << std::setw(2) << std::hex << (int) byte;
        }

        compare_ciphertext(ss.str());
    }

    const std::string &estream_test_case::plaintext() const {
        return _plaintext;
    }

    const std::string &estream_test_case::key() const {
        return _key;
    }

    const std::string &estream_test_case::iv() const {
        return _iv;
    }

    unsigned long estream_test_case::block_size() const {
        return _plaintext.size();
    }

    const json &estream_test_case::stream_config() const {
        return _stream_config;
    }

    std::unique_ptr<stream> estream_test_case::prepare_stream() {
        _stream_config["algorithm"] = _algorithm;
        _stream_config["round"] = _round;
        _stream_config["plaintext-type"]["outputs"] = {plaintext()};
        _stream_config["iv-type"]["outputs"] = {iv()};
        _stream_config["key-type"]["outputs"] = {key()};
        _stream_config["block-size"] = block_size();

        seed_seq_from<pcg32> seeder(seed1);

        return make_stream(_stream_config, seeder, block_size());
    }

    std::istream &operator>>(std::istream &input, estream_test_case &test_case) {
        test_case._plaintext.clear();
        test_case._ciphertext.clear();
        test_case._key.clear();
        test_case._iv.clear();

        input >> test_case._plaintext;
        input >> test_case._key;
        input >> test_case._iv;

        input.get();
        test_case.load_ciphertext();

        hex_string2string(test_case._plaintext);
        hex_string2string(test_case._key);
        hex_string2string(test_case._iv);

        std::uint32_t position;
        std::string ciphertext;

        return input;
    }

    void estream_test_case::test(std::unique_ptr<stream>&& encryptor, std::unique_ptr<estream_interface>& decryptor) const {
        std::stringstream ss;

        vec_view ciphertext = encryptor->next();

        char plain[ciphertext.size()];
        for (int i = 0; i < ciphertext.size(); i++) {
            plain[i] = 0;
        }

        decryptor->ECRYPT_keysetup((unsigned char *) _key.data(), _key.size() * 8, _iv.size() * 8);
        decryptor->ECRYPT_ivsetup((const u8 *) _iv.data());
        decryptor->ECRYPT_decrypt_bytes((const u8 *) ciphertext.data(), (u8*) plain, ciphertext.size());

        std::string plaintext;
        for(auto byte : plain) {
            plaintext.push_back(byte);
        }

        ASSERT_EQ(_plaintext, plaintext);

    }

    void estream_test_case::operator()() {
        _cipher->ECRYPT_init();

        while(_test_vectors >> *this) {
            test(_cipher);
            test_case::test(prepare_stream());
            test(prepare_stream(), _cipher);
        }
    }
}


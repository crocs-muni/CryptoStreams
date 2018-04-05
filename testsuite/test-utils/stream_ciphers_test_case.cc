#include <gtest/gtest.h>
#include <eacirc-core/external/pcg-cpp-0.98/pcg/pcg_random.hpp>
#include <eacirc-core/random.h>
#include <streams.h>
#include "stream_ciphers_test_case.h"

namespace testsuite {
    const json stream_cipher_test_case::base_config = {
            {"type",      "estream"},
            {"plaintext", {{"type", "test-stream"}}},
            {"iv",        {{"type", "test-stream"}}},
            {"key",       {{"type", "test-stream"}}},
    };

    void stream_cipher_test_case::test(std::unique_ptr<stream_interface> &encryptor) const {
        size_t size = _plaintext.size();
        std::vector<value_type> cipher(size);

        encryptor->keysetup(_key.data(), unsigned(_key.size() * 8), unsigned(_iv.size() * 8));
        encryptor->ivsetup(_iv.data());
        encryptor->encrypt_bytes(_plaintext.data(), cipher.data(), unsigned(_plaintext.size()));

        std::stringstream ss;

        for (auto byte : cipher) {
            ss << std::setfill('0') << std::setw(2) << std::hex << int(byte);
        }

        std::string ciphertext_str = ss.str();
        compare_ciphertext(ciphertext_str);
    }

    unsigned long stream_cipher_test_case::block_size() const {
        return _plaintext.size();
    }

    const json &stream_cipher_test_case::stream_config() const {
        return _stream_config;
    }

    std::unique_ptr<stream> stream_cipher_test_case::prepare_stream() {
        _stream_config["algorithm"] = _algorithm;
        _stream_config["round"] = _round;
        _stream_config["plaintext"]["outputs"] = _plaintext;
        _stream_config["iv"]["outputs"] = _iv;
        _stream_config["key"]["outputs"] = _key;
        _stream_config["block-size"] = block_size();

        seed_seq_from<pcg32> seeder(seed1);

        return make_stream(_stream_config, seeder, block_size());
    }

    void stream_cipher_test_case::test(std::unique_ptr<stream>&& encryptor, std::unique_ptr<stream_interface>& decryptor) const {
        vec_cview ciphertext = encryptor->next();

        std::vector<value_type> plain(ciphertext.size());

        decryptor->keysetup(_key.data(), _key.size() * 8, _iv.size() * 8);
        decryptor->ivsetup(_iv.data());
        decryptor->decrypt_bytes(ciphertext.data(), plain.data(), ciphertext.size());

        vec_cview plain_view = vec_cview(plain.cbegin(), plain.cend());
        ASSERT_EQ(plain_view.copy_to_vector(), _plaintext);
    }

    void stream_cipher_test_case::operator()() {
        _cipher->init();

        _test_vectors_tested = 0;
        while(_test_vectors >> *this) {
            _test_vectors_tested++;
            test(_cipher);
            auto stream = prepare_stream();
            test_case::test(stream);
            test(prepare_stream(), _cipher);
        }

        std::cout << "Number of test vectors tested for function: \"" << _algorithm << "\"[" << _round << "] is: " << _test_vectors_tested << std::endl;
    }

    void stream_cipher_test_case::load_ciphertext(std::istream& stream) {
        _ciphertext.clear();
        std::string cipher_line;
        std::getline(stream, cipher_line);
        // Hexadecimal to lower case, in order to be compatible with std::hex
        std::transform(cipher_line.begin(), cipher_line.end(), cipher_line.begin(), ::tolower);

        if (std::any_of(cipher_line.cbegin(), cipher_line.cend(), isspace)) {
            std::stringstream cipher_stream(cipher_line);
            std::size_t position;
            std::string ciphertext;

            while (cipher_stream >> position) {
                cipher_stream >> ciphertext;
                _ciphertext.push_back(std::make_tuple(position, ciphertext));
            }

            return;
        }
        _ciphertext.push_back(std::make_tuple(0, cipher_line));
    }

    void stream_cipher_test_case::compare_ciphertext(const std::string& actual) const {
        for (auto tuple : _ciphertext) {
            ASSERT_EQ(actual.substr(std::get<0>(tuple) * 2,
                                    std::get<1>(tuple).size()),
                      std::get<1>(tuple)); // *2 because sequence starts at std::get<0>(tuple) byte, but we are storing each byte on two bytes f. e. FF
        }
    }

    std::istream &operator>>(std::istream &input, stream_cipher_test_case &test_case) {
        std::string plaintext;
        std::string key;
        std::string iv;

        input >> plaintext;
        input >> key;
        input >> iv;

        // fixes empty IV
        if (iv == "empty") {
            iv = "";
        }

        input.get(); // read the newline
        test_case.load_ciphertext(input);
        test_case.update_test_vector(hex_string_to_binary(plaintext),
                                     hex_string_to_binary(key),
                                     hex_string_to_binary(iv));

        return input;
    }
}


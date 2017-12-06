#include <memory>
#include <gtest/gtest.h>
#include <streams.h>
#include <streams/block/block_factory.h>
#include "block_test_case.h"

namespace testsuite {

    const json block_test_case::base_config = {
        {"type", "block"},
        {"init-frequency", "only-once"},
        {"plaintext", {{"type", "test-stream"}}},
        {"iv", {{"type", "test-stream"}}},
        {"key", {{"type", "test-stream"}}}
    };

    void block_test_case::test() const {
        std::unique_ptr<block::block_cipher> encryptor = block::make_block_cipher(_algorithm, _round, _plaintext.size(), true);

        encryptor->keysetup(_key.data(), _key.size());
        if (_load_iv)
            encryptor->keysetup(_iv.data(), _iv.size());

        std::vector<value_type> output_ciphertext(_ciphertext.size());
        encryptor->encrypt(_plaintext.data(), output_ciphertext.data());

        ASSERT_EQ(output_ciphertext, _ciphertext);
    }

    void block_test_case::test(std::unique_ptr<stream>&& block_stream) const {
        std::unique_ptr<block::block_cipher> decryptor = block::make_block_cipher(_algorithm, _round, _plaintext.size(), false);
        vec_cview ciphertext = block_stream->next();

        decryptor->keysetup(_key.data(), _key.size());
        if (_load_iv)
            decryptor->keysetup(_iv.data(), _iv.size());

        std::vector<value_type> plain(_plaintext.size());
        decryptor->decrypt(ciphertext.data(), plain.data());

        ASSERT_EQ(plain, _plaintext);
    }

    void block_test_case::operator()() {
        _test_vectors_tested = 0;

        while (_test_vectors >> *this) {
            _test_vectors_tested++;
            test(); // encryption
            auto stream = prepare_stream();
            test_case::test(stream); // encryption streams
            test(prepare_stream()); // decryption
        }



        std::cout << "Number of test vectors tested for function: \"" << _algorithm << "\"[" << _round << "] is: " << _test_vectors_tested << std::endl;
    }

    bool block_test_case::load_iv() const
    {
        return _load_iv;
    }

    std::unique_ptr<stream> block_test_case::prepare_stream() {
        std::size_t block_size =  _ciphertext.size();
        _stream_config["algorithm"] = _algorithm;
        _stream_config["round"] = _round;
        _stream_config["plaintext"]["outputs"] = _plaintext;
        _stream_config["key"]["outputs"] = _key;
        _stream_config["key-size"] = _key.size();
        if (_load_iv) {
            _stream_config["mode"] = ""; // TODO: write mode here
            _stream_config["iv"]["outputs"] = _iv;
            _stream_config["iv-size"] = _iv.size();
        } else {
            _stream_config["iv"]["outputs"] = _iv;
        }
        _stream_config["block-size"] = block_size;

        seed_seq_from<pcg32> seeder(seed1);
        return make_stream(_stream_config, seeder, block_size);
    }

    std::istream &operator>>(std::istream &input, block_test_case &test_case) {
        std::string str_plaintext;
        std::string str_ciphertext;
        std::string str_iv;
        std::string str_key;

        input >> str_key;
        input >> str_plaintext;
        input >> str_ciphertext;
        if (test_case.load_iv())
            input >> str_iv;

        test_case.update_test_vector(hex_string_to_binary(str_plaintext),
                                     hex_string_to_binary(str_ciphertext),
                                     hex_string_to_binary(str_key),
                                     hex_string_to_binary(str_iv));

        return input;
    }

}

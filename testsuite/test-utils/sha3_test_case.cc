#include <ios>
#include <sstream>
#include <memory>
#include <iostream>
#include <iomanip>
#include <gtest/gtest.h>
#include <streams.h>
#include "sha3_test_case.h"

namespace testsuite {

    const json sha3_test_case::base_config = {
            {"type", "sha3"},
            {"source", {{"type", "test-stream"}}}
    };

    void sha3_test_case::test(std::unique_ptr<sha3_interface> &hasher) const {
        std::size_t hash_size = _ciphertext.size();

        std::vector<value_type> hash(hash_size);

        using std::to_string;

        int status = hasher->Init(int(hash_size * 8));
        if (status != 0)
            throw std::runtime_error("cannot initialize hash (code: " + to_string(status) + ")");

        status = hasher->Update(_plaintext.data(), _length);
        if (status != 0)
            throw std::runtime_error("cannot update the hash (code: " + to_string(status) + ")");

        status = hasher->Final(hash.data());
        if (status != 0)
            throw std::runtime_error("cannot finalize the hash (code: " + to_string(status) + ")");

        ASSERT_EQ(hash, _ciphertext);
    }

    void sha3_test_case::operator()() {
        std::unique_ptr<sha3_interface> hasher = sha3_factory::create(_algorithm, _round);

        _test_vectors_tested = 0;

        while (_test_vectors >> *this) {
            _test_vectors_tested++;
            test(hasher);
            if (length() != 0 && length() % 8 == 0) { // unfortunately our sha3 streams are built so that it fits only multiple of 8
                auto stream = prepare_stream();
                test_case::test(stream);
            }
        }

        std::cout << "Number of test vectors tested for function: \"" << _algorithm << "\"[" << _round << "] is: " << _test_vectors_tested << std::endl;
    }

    const size_t& sha3_test_case::length() const {
        return _length;
    }

    std::unique_ptr<stream> sha3_test_case::prepare_stream() {
        std::size_t hash_size =  _ciphertext.size();
        _stream_config["algorithm"] = _algorithm;
        _stream_config["round"] = _round;
        _stream_config["source"]["outputs"] = _plaintext;
        _stream_config["hash-bitsize"] = hash_size * 8;

        seed_seq_from<pcg32> seeder(seed1);
        return make_stream(_stream_config, seeder, hash_size);
    }

    void sha3_test_case::update_test_vector(std::vector<value_type> &&plaintext, std::vector<value_type> &&ciphertext) {
        _plaintext = plaintext;
        _ciphertext = ciphertext;
    }

    std::istream &operator>>(std::istream &input, sha3_test_case &test_case) {
        std::string str_plaintext;
        std::string str_hash;

        input >> test_case._length;
        input >> str_plaintext;
        input >> str_hash;

        test_case.update_test_vector(hex_string_to_binary(str_plaintext),
                                     hex_string_to_binary(str_hash));

        return input;
    }

}

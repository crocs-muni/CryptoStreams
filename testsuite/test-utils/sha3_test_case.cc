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
        std::size_t hash_size = std::get<1>(_ciphertext[0]).size();

        unsigned char hash[hash_size/2];
        hasher->Hash(hash_size * 4, (unsigned char*) _plaintext.data(), _length, hash);

        std::stringstream ss;

        for (auto byte : hash) {
            ss << std::setfill('0') << std::setw(2) << std::hex << (int) byte;
        }

        compare_ciphertext(ss.str());
    }

    void sha3_test_case::operator()() {
        std::unique_ptr<sha3_interface> hasher = sha3_factory::create(_algorithm, _round);

        while (_test_vectors >> *this) {
            test(hasher);
            if (length() != 0 && length() % 8 == 0) { // unfortunately our sha3 streams are built so that it fits only multiple of 8
                test_case::test(prepare_stream());
            }
        }
    }

    const size_t& sha3_test_case::length() const {
        return _length;
    }

    const std::string& sha3_test_case::input() const {
        return _plaintext;
    }

    std::unique_ptr<stream> sha3_test_case::prepare_stream() {
        std::size_t hash_size =  std::get<1>(_ciphertext[0]).size();
        _stream_config["algorithm"] = _algorithm;
        _stream_config["round"] = _round;
        _stream_config["source"]["outputs"] = {input()};
        _stream_config["hash-bitsize"] = hash_size * 4;

        seed_seq_from<pcg32> seeder(seed1);
        return make_stream(_stream_config, seeder, hash_size / 2);
    }

    std::istream &operator>>(std::istream &input, sha3_test_case &test_case) {
        test_case._length = 0;
        test_case._plaintext.clear();

        input >> test_case._length;
        input >> test_case._plaintext;
        input.get();
        test_case.load_ciphertext();

        hex_string2string(test_case._plaintext); //

        return input;
    }

}
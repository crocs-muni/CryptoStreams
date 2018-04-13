#include "hash_test_case.h"
#include <gtest/gtest.h>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <sstream>
#include <streams.h>

namespace testsuite {

void hash_test_case::test() const {
    std::unique_ptr<hash::hash_interface> hasher =
        hash::hash_factory::create(_algorithm, unsigned(_round));
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

void hash_test_case::operator()() {
    _test_vectors_tested = 0;

    while (_test_vectors >> *this) {
        _test_vectors_tested++;
        test();
        if (length() != 0 &&
            length() % 8 ==
                0) { // unfortunately our sha3 streams are built so that it fits only multiple of 8
            auto stream = prepare_stream();
            test_case::test(stream);
        }
    }

    std::cout << "Number of test vectors tested for function: \"" << _algorithm << "\"[" << _round
              << "] is: " << _test_vectors_tested << std::endl;
}

const size_t &hash_test_case::length() const {
    return _length;
}

std::unique_ptr<stream> hash_test_case::prepare_stream() {
    std::size_t hash_size = _ciphertext.size();
    json config_instance = {{"type", "hash"}, {"source", {{"type", "test_stream"}}}};
    config_instance["algorithm"] = _algorithm;
    config_instance["round"] = _round;
    config_instance["input_size"] = _plaintext.size();
    config_instance["source"]["outputs"] = _plaintext;
    config_instance["hash_size"] = hash_size;

    seed_seq_from<pcg32> seeder(seed1);
    return make_stream(config_instance, seeder, hash_size);
}

std::istream &operator>>(std::istream &input, hash_test_case &test_case) {
    std::string str_plaintext;
    std::string str_hash;

    input >> test_case._length;
    input >> str_plaintext;
    input >> str_hash;

    test_case.update_test_vector(hex_string_to_binary(str_plaintext),
                                 hex_string_to_binary(str_hash));

    return input;
}

} // namespace testsuite

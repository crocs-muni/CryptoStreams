#include <gtest/gtest.h>
#include "estream_test_case.h"

void testsuite::estream_test_case::operator()(std::unique_ptr<estream_interface> &encryptor) {

    testsuite::hex_string2string(_plaintext);
    testsuite::hex_string2string(_key);
    testsuite::hex_string2string(_iv);

    encryptor->ECRYPT_keysetup((unsigned char*) _key.data(), _key.size() * 8, _iv.size() * 8);
    encryptor->ECRYPT_ivsetup((const u8 *) _iv.data());

    unsigned char cipher[_plaintext.size()];


    for(int i = 0; i < _plaintext.size(); i++) {
        cipher[i] = 0;
    }

    encryptor->ECRYPT_encrypt_bytes((const u8 *) _plaintext.data(), cipher, _plaintext.size());

    std::stringstream ss;

    for (auto byte : cipher) {
        ss << std::setfill('0') << std::setw(2) << std::hex << (int) byte;
    }

    for(auto tuple : _ciphertext) {
        ASSERT_EQ(ss.str().substr(std::get<0>(tuple) * 2, std::get<1>(tuple).size()), std::get<1>(tuple)); // *2 because sequence starts at std::get<0>(tuple) byte, but we are storing each byte on two bytes f. e. FF
    }
}


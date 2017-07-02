#ifndef EACIRC_STREAMS_ESTREAM_TEST_CASE_H
#define EACIRC_STREAMS_ESTREAM_TEST_CASE_H

#include <istream>
#include <memory>
#include <streams/estream/estream_interface.h>
#include <algorithm>
#include <vector>
#include <sstream>
#include "common_functions.h"

namespace testsuite {
    class estream_test_case {

    private:
        std::string _plaintext;
        std::string _key;
        std::string _iv;
        std::vector<std::tuple<std::int32_t , std::string>> _ciphertext;

    public:

        void operator()(std::unique_ptr<estream_interface>& encryptor);

        //void operator()(std::unique_ptr<stream>& stream);

        friend std::istream& operator>>(std::istream& input, estream_test_case& test_case) {
            test_case._plaintext.clear();
            test_case._ciphertext.clear();
            test_case._key.clear();
            test_case._iv.clear();


            input >> test_case._plaintext;
            input >> test_case._key;
            input >> test_case._iv;

            std::string cipher_line;
            std::getline(input, cipher_line);
            std::getline(input, cipher_line);
            std::stringstream cipher_stream(cipher_line);

            std::uint32_t position;
            std::string ciphertext;

            while (cipher_stream >> position) {
                cipher_stream >> ciphertext;

                // Hexadecimal to lower case, in order to be compatible with std::hex
                std::transform(ciphertext.begin(), ciphertext.end(), ciphertext.begin(), ::tolower);
                test_case._ciphertext.push_back(std::make_tuple(position, ciphertext));
            }


            return input;
        }
    };
}



#endif //EACIRC_STREAMS_ESTREAM_TEST_CASE_H

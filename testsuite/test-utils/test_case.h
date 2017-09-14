//
// Created by mhajas on 7/27/17.
//

#ifndef EACIRC_STREAMS_TEST_CASE_H
#define EACIRC_STREAMS_TEST_CASE_H

#include <string>
#include <algorithm>
#include <stream.h>
#include "common_functions.h"
#include "gtest/gtest.h"


namespace testsuite {

    const static seed seed1 = seed::create("1fe40505e131963c");

    class test_case {

    protected:

        const std::string _algorithm;
        const std::size_t _round;
        const std::string _competition;

        std::uint64_t _test_vectors_tested = 0;

        std::ifstream _test_vectors;

        std::string _plaintext;
        std::vector<std::tuple<std::int32_t , std::string>> _ciphertext;

        void load_ciphertext() {
            _ciphertext.clear();
            std::string cipher_line;
            std::getline(_test_vectors, cipher_line);
            // Hexadecimal to lower case, in order to be compatible with std::hex
            std::transform(cipher_line.begin(), cipher_line.end(), cipher_line.begin(), ::tolower);

            for(char c : cipher_line) {
                if (isspace(c)) {
                    std::stringstream cipher_stream(cipher_line);
                    std::size_t position;
                    std::string ciphertext;

                    while (cipher_stream >> position) {
                        cipher_stream >> ciphertext;
                        _ciphertext.push_back(std::make_tuple(position, ciphertext));
                    }

                    return;
                }
            }
            _ciphertext.push_back(std::make_tuple(0, cipher_line));
        }

        void compare_ciphertext(std::string&& actual) const {
            for (auto tuple : _ciphertext) {
                ASSERT_EQ(actual.substr(std::get<0>(tuple) * 2, std::get<1>(tuple).size()), std::get<1>(
                        tuple)); // *2 because sequence starts at std::get<0>(tuple) byte, but we are storing each byte on two bytes f. e. FF
            }
        }

        void test(std::unique_ptr<stream>&& s) const {
            std::stringstream ss;

            for (auto byte : s->next()) {
                ss << std::setfill('0') << std::setw(2) << std::hex << (int) byte;
            }

            compare_ciphertext(ss.str());
        }

        static std::string get_test_vectors_filename(std::string& algorithm, std::size_t round, std::string& competition) {
            return "resources/test-resources/" + competition + "/" + algorithm +
                   "/test-vectors-" + std::to_string(round) + ".txt";
        }

    public:

        test_case(std::string& algorithm, std::size_t round, std::string&& competition)
                : _algorithm(algorithm)
                , _round(round)
                , _competition(competition)
                , _test_vectors(get_test_vectors_filename(algorithm, round, competition),  std::ifstream::in)
                {
                    if (!_test_vectors.is_open()) {
                        throw std::runtime_error("Not able to open file: \"" + get_test_vectors_filename(algorithm, round, competition) + "\"");
                    }
                }

        virtual ~test_case() {
            _test_vectors.close();
        };


    };

}

#endif //EACIRC_STREAMS_TEST_CASE_H

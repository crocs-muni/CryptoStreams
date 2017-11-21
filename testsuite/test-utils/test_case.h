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

    /** seed is not actually used but some functions needs it as argument **/
    const static seed seed1 = seed::create("1fe40505e131963c");

    /**
     * this is abstract parent of all test cases, contains all functions and variables
     * which cipher suites test vectors has in common e.g. plaintext, ciphertext, rounds etc.
     */
    class test_case {

    protected:
        /**
         * Algorithm name, which is used in cipher suites factories so that it is possible to create
         * instance from it
         */
        const std::string _algorithm;

        /** Number of round of function for current test vectors **/
        const std::size_t _round;

        /** Name of suite e.g. sha3 or estream etc. **/
        const std::string _suite_name;

        /** This counts number of test vectors tested in operator() **/
        std::uint64_t _test_vectors_tested = 0;

        /** File stream with test vectors **/
        std::ifstream _test_vectors;

        /** Plaintext, should be already converted from pure string to bytes representation **/
        std::string _plaintext;

        /**
         * cipher text in form of tuples <index where begins ciphertext, ciphertext>
         * for long ciphertext testvectors contains more tuples, if whole ciphertext is given
         * this structure contains only one tuple <0, ciphertext>
         * **/
        std::vector<std::tuple<std::int32_t , std::string>> _ciphertext;

        /**
         * Load ciphertext from stream, stream must be in separate line and
         * should be in format ciphertext, or index1 ciphertext_which_begins_in_index1 index2 ..... etc.
         * @param stream to read from
         */
        void load_ciphertext(std::istream& stream) {
            _ciphertext.clear();
            std::string cipher_line;
            std::getline(stream, cipher_line);
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

        /**
         * Compare ciphertext stored in _ciphertext with parameter actual
         * for comparing it uses google test function
         * @param actual
         */
        void compare_ciphertext(std::string&& actual) const {
            for (auto tuple : _ciphertext) {
                ASSERT_EQ(actual.substr(std::get<0>(tuple) * 2, std::get<1>(tuple).size()), std::get<1>(
                        tuple)); // *2 because sequence starts at std::get<0>(tuple) byte, but we are storing each byte on two bytes f. e. FF
            }
        }

        /**
         * Auxiliary function which generates filepath to testvectors for arguments
         * @param algorithm
         * @param round
         * @param competition
         * @return filepath
         */
        static std::string get_test_vectors_filename(std::string& algorithm, std::size_t round, std::string& competition) {
            return "resources/test-resources/" + competition + "/" + algorithm +
                   "/test-vectors-" + std::to_string(round) + ".txt";
        }

    public:

        test_case(std::string& algorithm, std::size_t round, std::string&& competition)
                : _algorithm(algorithm)
                , _round(round)
                , _suite_name(competition)
                , _test_vectors(get_test_vectors_filename(algorithm, round, competition),  std::ifstream::in)
        {
            if (!_test_vectors.is_open()) {
                throw std::runtime_error("Not able to open file: \"" + get_test_vectors_filename(algorithm, round, competition) + "\"");
            }
        }

        /**
         * Generates next output from stream and compare it with expected output
         * @param s stream
         */
        void test(std::unique_ptr<stream>&& s) const {
            std::stringstream ss;

            for (auto byte : s->next()) {
                ss << std::setfill('0') << std::setw(2) << std::hex << (int) byte;
            }

            compare_ciphertext(ss.str());
        }

        /**
         * Create new instance of stream for current function
         * First it creates JSON configuration of stream and
         * then create new stream using make_stream function
         * @return Pointer to instance of stream
         */
        virtual std::unique_ptr<stream> prepare_stream() = 0;

        /**
         * test case is actually a functor which can done whole testing
         * It is necessary to have function name and round number and it
         * will step by step load all test vectors and test each with raw
         * function and stream
         */
        virtual void operator()() = 0;

        virtual ~test_case() {
            _test_vectors.close();
        };
    };

}

#endif //EACIRC_STREAMS_TEST_CASE_H

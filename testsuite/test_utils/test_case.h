//
// Created by mhajas on 7/27/17.
//

#pragma once

#include "common_functions.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <eacirc-core/seed.h>
#include <fstream>
#include <stream.h>
#include <string>

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

    /** This counts number of test vectors tested in operator() **/
    std::uint64_t _test_vectors_tested = 0;

    /** File stream with test vectors **/
    std::ifstream _test_vectors;

    /** Plaintext, should be already converted from pure string to bytes representation **/
    std::vector<value_type> _plaintext;

    /** cipher text in form of byte vector **/
    std::vector<value_type> _ciphertext;

    /**
     * Auxiliary function which generates filepath to testvectors for arguments
     * @param algorithm
     * @param round
     * @param competition
     * @return filepath
     */
    static std::string get_test_vectors_filename(const std::string &algorithm,
                                                 const std::size_t round,
                                                 const std::string &competition) {
        return "resources/test-resources/" + competition + "/" + algorithm + "/test-vectors-" +
               std::to_string(round) + ".txt";
    }

public:
    test_case(const std::string &algorithm, const std::size_t round, const std::string &competition)
        : _algorithm(algorithm)
        , _round(round)
        , _test_vectors(get_test_vectors_filename(algorithm, round, competition),
                        std::ifstream::in) {
        if (!_test_vectors.is_open()) {
            throw std::runtime_error("Not able to open file: \"" +
                                     get_test_vectors_filename(algorithm, round, competition) +
                                     "\"");
        }
    }

    /**
     * Generates next output from stream and compare it with expected output
     * @param s stream
     */
    void test(std::unique_ptr<stream> &s) const {
        vec_cview actual = s->next();
        ASSERT_EQ(make_cview(_ciphertext), actual);
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

    virtual ~test_case() { _test_vectors.close(); }
};

} // namespace testsuite

#pragma once

#include <streams/block/block_cipher.h>
#include "test_case.h"

namespace testsuite {
    /** test case for functions from sha3 competition **/
    class block_test_case : test_case {

    private:
        /** Private key for current test vector **/
        std::vector<value_type> _key;

        /** Initialization vector for current test vector **/
        std::vector<value_type> _iv;

        /** whether we use IV **/
        bool _load_iv;

        /** JSON configuration for making stream using make_stream() **/
        json _stream_config;

    public:
        /**
         * base JSON configuration which all estream functions has in common
         * _stream_config is created based on this, but with addition of
         * function specific properties e.g. name of function, number of rounds
         */
        const static json base_config;

        block_test_case(const std::string&& algorithm, const std::size_t round, const bool load_iv=false)
                : test_case(algorithm, round, "block")
                , _iv(0) // empty IV if it is not used
                , _load_iv(load_iv)
                , _stream_config(base_config)
                {}


        /** Setter for single test vector **/
        void update_test_vector(const std::vector<value_type> &&plaintext,
                                const std::vector<value_type> &&ciphertext,
                                const std::vector<value_type> &&key,
                                const std::vector<value_type> &&iv) {
            _plaintext = plaintext;
            _ciphertext = ciphertext;
            _key = key;
            if (_load_iv)
                _iv = iv;
        }

        /**
         * Create new instance of stream for current function
         * First it creates JSON configuration of stream and
         * then create new stream using make_stream function
         * @return Pointer to instance of stream
         */
        std::unique_ptr<stream> prepare_stream() override;

        /**
         * Test raw function with current test vector
         * @param encryptor
         */
        void test() const;

        /**
         * Test stream for current function with current test vector
         * including decryption of output of stream
         * @param encryptor Stream for current function
         * @param decryptor Pointer to raw function instance
         */
        void test(std::unique_ptr<stream>&& encryptor) const;

        /**
         * test case is actually a functor which can done whole testing
         * It is necessary to have function name and round number and it
         * will step by step load all test vectors and test each with raw
         * function and stream
         */
        void operator()() override;

        /**
         * Reads test vector from input stream
         *
         * STRUCTURE OF TEST VECTORS (All present in test-resources/block/{function-name}/test-vectors-{round-number}.txt)
         * {key in hex}
         * {plaintext in hex}
         * {ciphertext in hex}
         * {initialization-vector in hex} # Only in case _load_iv is true
         * {newline}
         * {key in hex}
         * {plaintext in hex}
         * {ciphertext in hex}
         * {initialization-vector in hex} # Only in case _load_iv is true
         *
         * @param input stream
         * @param test_case test_case instance
         * @return input stream
         */
        friend std::istream &operator>>(std::istream &input, block_test_case &test_case);

        bool load_iv() const;
    };
}

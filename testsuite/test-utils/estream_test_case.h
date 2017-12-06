#ifndef EACIRC_STREAMS_ESTREAM_TEST_CASE_H
#define EACIRC_STREAMS_ESTREAM_TEST_CASE_H

#include <istream>
#include <memory>
#include <algorithm>
#include <vector>
#include <sstream>
#include <stream.h>
#include <streams/estream/estream_interface.h>
#include <streams/estream/estream_cipher.h>
#include "common_functions.h"
#include "test_case.h"

namespace testsuite {
    /**
     * Class represents one test_case with one test vector
     * class support testing of estream function directly including decryption
     * and also testing of stream with
     */
    class estream_test_case : test_case {

    private:

        void compare_ciphertext(const std::string &actual) const;

        /**
         * cipher text in form of tuples <index where begins ciphertext, ciphertext>
         * for long ciphertext testvectors contains more tuples, if whole ciphertext is given
         * this structure contains only one tuple <0, ciphertext>
         * **/
        std::vector<std::tuple<std::int32_t , std::string>> _ciphertext;

        /** Private key for current test vector **/
        std::vector<value_type> _key;

        /** Initialization vector for current test vector **/
        std::vector<value_type> _iv;

        /** JSON configuration for making stream using make_stream() **/
        json _stream_config;

        /** Pointer to raw function instance **/
        std::unique_ptr<estream_interface> _cipher;
    public:

        /**
         * base JSON configuration which all estream functions has in common
         * _stream_config is created based on this, but with addition of
         * function specific properties e.g. name of function, number of rounds
         */
        const static json base_config;

        estream_test_case(const std::string&& algorithm, const std::size_t round)
                : test_case(algorithm, round, "estream")
                , _stream_config(base_config)
                , _cipher(create_estream_cipher(_algorithm,
                                        core::optional<unsigned>{unsigned(_round)}))
        {}

        /** Setter for single test vector **/
        void update_test_vector(const std::vector<value_type> &&plaintext,
                                const std::vector<value_type> &&key,
                                const std::vector<value_type> &&iv) {
            _plaintext = plaintext;
            _key = key;
            _iv = iv;
        }

        /**
         * Loads ciphertext from stream, stream must be in separate line and
         * should be in format ciphertext, or index1 ciphertext_which_begins_in_index1 index2 ..... etc.
         * @param stream to read from
         */
        void load_ciphertext(std::istream& stream);

        /** Getter for current function configuration **/
        const json& stream_config() const;

        /** Getter for block size **/
        unsigned long block_size() const;

        /**
         * Test raw function with current test vector
         * @param encryptor
         */
        void test(std::unique_ptr<estream_interface>& encryptor) const;

        /**
         * Test stream for current function with current test vector
         * including decryption of output of stream
         * @param encryptor Stream for current function
         * @param decryptor Pointer to raw function instance
         */
        void test(std::unique_ptr<stream>&& encryptor, std::unique_ptr<estream_interface>& decryptor) const;

        /**
         * Create new instance of stream for current function
         * First it creates JSON configuration of stream and
         * then create new stream using make_stream function
         * @return Pointer to instance of stream
         */
        std::unique_ptr<stream> prepare_stream();

        /**
         * test case is actually a functor which can done whole testing
         * It is necessary to have function name and round number and it
         * will step by step load all test vectors and test each with raw
         * function and stream
         */
        void operator()();

        /**
         * Reads test vector from input stream
         *
         * STRUCTURE OF TEST VECTORS (All present in test-resources/estream/{function-name}/test-vectors-{round-number}.txt)
         * {plaintext in hex}
         * {key in hex}
         * {initialization vector}
         * {ciphertext}
         * {plaintext2 in hex}
         * {key2 in hex}
         * {initialization vector2}
         * {ciphertext2}
         *
         * STRUCTURE OF CIPHERTEXT
         * two options
         * - whole plaintext
         * - tuples (index where ciphertext begins) (ciphretext)
         *     for example "0 FF 5 EE" means that first byte is FF and sixth byte is EE
         *
         * @param input stream
         * @param test_case test_case instance
         * @return input stream
         */
        friend std::istream& operator>>(std::istream& input, estream_test_case& test_case);
    };
}



#endif //EACIRC_STREAMS_ESTREAM_TEST_CASE_H

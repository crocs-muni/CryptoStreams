#ifndef EACIRC_STREAMS_ESTREAM_TEST_CASE_H
#define EACIRC_STREAMS_ESTREAM_TEST_CASE_H

#include <istream>
#include <memory>
#include <streams/estream/estream_interface.h>
#include <algorithm>
#include <vector>
#include <sstream>
#include <stream.h>
#include <eacirc-core/seed.h>
#include "common_functions.h"
#include "test_case.h"

namespace testsuite {
    class estream_test_case : test_case {

    private:
        std::string _key;
        std::string _iv;

        json _stream_config;
        std::unique_ptr<estream_interface> _cipher;
    public:
        const static json base_config;

        estream_test_case(std::string&& algorithm, std::size_t round)
                : test_case(algorithm, round, "estream")
                , _stream_config(base_config)
                , _cipher(create_cipher(_algorithm,
                                        core::optional<unsigned>{unsigned(_round)}))
        {}

        const std::string& plaintext() const;

        const std::string& key() const;

        const std::string& iv() const;

        const json& stream_config() const;

        unsigned long block_size() const;

        void test(std::unique_ptr<estream_interface>& encryptor) const;

        void test(std::unique_ptr<stream>&& encryptor, std::unique_ptr<estream_interface>& decryptor) const;

        std::unique_ptr<stream> prepare_stream();

        void operator()();

        friend std::istream& operator>>(std::istream& input, estream_test_case& test_case);
    };
}



#endif //EACIRC_STREAMS_ESTREAM_TEST_CASE_H

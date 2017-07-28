#ifndef EACIRC_STREAMS_SHA3_TEST_CASE_H
#define EACIRC_STREAMS_SHA3_TEST_CASE_H

#include <streams/sha3/sha3_interface.h>
#include <eacirc-core/seed.h>
#include <streams/sha3/sha3_factory.h>
#include "test_case.h"

namespace testsuite {

    class sha3_test_case : test_case {

    private:
        size_t _length;
        json _stream_config;

    public:
        const static json base_config;

        const size_t& length() const;
        const std::string& input() const;

        sha3_test_case(std::string&& algorithm, std::size_t round)
                : test_case(algorithm, round, "sha3")
                , _stream_config(base_config)
                {}

        std::unique_ptr<stream> prepare_stream();

        void test(std::unique_ptr<sha3_interface> &hasher) const;

        void operator()();

        friend std::istream &operator>>(std::istream &input, sha3_test_case &test_case);

    };
}


#endif //EACIRC_STREAMS_SHA3_TEST_CASE_H

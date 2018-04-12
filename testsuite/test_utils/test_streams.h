#pragma once

#include "stream.h"
#include "common_functions.h"

namespace testsuite {

    /** Stream for testing which return values given by JSON configuration **/
    struct test_stream : stream {
        test_stream(const json& config);

        vec_cview next() override;

    private:
        /** view for the data, which is provided to next() **/
        std::vector<value_type> _data;
    };
}


#pragma once

#include "common_functions.h"
#include "stream.h"

namespace testsuite {

/** Stream for testing which return values given by JSON configuration **/
struct test_stream : stream {
    test_stream(const json &config);
    test_stream(const std::initializer_list<std::vector<value_type >> &data);

    vec_cview next() override;

private:
    /** view for the data, which is provided to next() **/
    std::vector<std::vector<value_type >> _data;
    size_t _position;
};

} // namespace testsuite

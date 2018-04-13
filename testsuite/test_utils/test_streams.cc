#include "test_streams.h"

testsuite::test_stream::test_stream(const json &config)
    : stream(config.at("outputs").get<std::vector<value_type>>().size())
    , _data(config.at("outputs").get<std::vector<value_type>>()) {}

vec_cview testsuite::test_stream::next() {
    return make_cview(_data);
}

#include "test_streams.h"

testsuite::test_stream::test_stream(const json &config)
    : stream(config.at("outputs").get<std::vector<std::vector<value_type>>>().at(0).size())
    , _data(config.at("outputs").get<std::vector<std::vector<value_type>>>())
    , _position(0) {}


vec_cview testsuite::test_stream::next() {
    size_t current_position = _position;

    if (++_position >= _data.size()) {
        _position = 0;
    }
    return make_cview(_data.at(current_position));
}

testsuite::test_stream::test_stream(const std::initializer_list<std::vector<value_type>> &data)
    : stream((*data.begin()).size())
    , _data(data)
    , _position(0) {}

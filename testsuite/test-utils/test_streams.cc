#include "test_streams.h"


testsuite::test_stream::test_stream(const json &config, std::size_t osize)
    : stream(osize),
    _data(osize),
    _position(0)
{
    for (auto output : config.at("outputs")) {
        _values.push_back(output);
    }
}

vec_view testsuite::test_stream::next() {
    std::string current_output = _values[_position++];

    for (int i = 0; i < current_output.size() / 2; i++) {
        _data[i] = (hex2char(current_output.at(2 * i)) << 4) + hex2char(current_output.at(2 * i + 1));
    }

    return make_cview(_data);
}

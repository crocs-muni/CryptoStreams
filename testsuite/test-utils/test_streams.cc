#include "test_streams.h"


testsuite::test_stream::test_stream(const json &config)
    : stream(config.at("outputs").at(0).get<std::string>().size())
    , _data(config.at("outputs").at(0).get<std::string>().size())
    , _position(0)
{
    for (auto output : config.at("outputs")) {
        _values.push_back(output);
    }
}

vec_view testsuite::test_stream::next() {
    std::string current_output = _values[_position++];

    for (int i = 0; i < current_output.size(); i++) {
        _data[i] = current_output[i];
    }

    return make_cview(_data);
}

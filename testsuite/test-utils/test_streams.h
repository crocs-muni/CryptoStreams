#ifndef EACIRC_STREAMS_TEST_STREAMS_H
#define EACIRC_STREAMS_TEST_STREAMS_H

#include "stream.h"
#include "common_functions.h"

namespace testsuite {

    struct test_stream : stream {
        test_stream(const json& config, std::size_t osize);

        vec_view next() override;

    private:
        std::vector<value_type> _data;
        std::vector<std::string> _values;
        int _position;
    };
}


#endif //EACIRC_STREAMS_TEST_STREAMS_H

#ifndef EACIRC_STREAMS_TEST_STREAMS_H
#define EACIRC_STREAMS_TEST_STREAMS_H

#include "stream.h"
#include "common_functions.h"

namespace testsuite {

    /** Stream for testing which return values given by JSON configuration **/
    struct test_stream : stream {
        test_stream(const json& config);

        vec_view next() override;

    private:
        /** actual data container, which is provided by next() **/
        std::vector<value_type> _data;

        /** vector of values read from JSON configuration **/
        std::vector<std::string> _values;

        /** Last position from _values which was returned **/
        int _position;
    };
}


#endif //EACIRC_STREAMS_TEST_STREAMS_H

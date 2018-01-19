//
// Created by mhajas on 5/4/17.
//

#include "gtest/gtest.h"
#include "stream.h"
#include "streams.h"
#include <eacirc-core/seed.h>
#include <testsuite/test-utils/test_case.h>

const static int testing_size = 1536;

TEST(true_stream, basic_test) {
    std::unique_ptr<true_stream> tstream = std::make_unique<true_stream>(testing_size);

    int i = 0;
    for (auto it : tstream->next()) {
        ASSERT_EQ(it, std::numeric_limits<std::uint8_t>::max());
        i++;
    }

    ASSERT_EQ(i, testing_size);
}

TEST(false_streams, basic_test) {
    std::unique_ptr<false_stream> fstream = std::make_unique<false_stream>(testing_size);

    int i = 0;
    for (auto it : fstream->next()) {
        ASSERT_EQ(it, std::numeric_limits<std::uint8_t>::min());
        i++;
    }

    ASSERT_EQ(i, testing_size);
}

TEST(counter_stream, basic_test) {
    std::unique_ptr<counter> cstream = std::make_unique<counter>(testing_size/256);

    for(int j = 1; j <= 65536 + 2; j++) { // 2^16 + 2
        auto data = cstream->next();
        std::vector<value_type> non_const(data.begin(), data.end());
        auto t = reinterpret_cast<uint32_t*>(non_const.data());

        ASSERT_EQ(j, *t);
    }

}

TEST(sac_streams, basic_test) {
    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unique_ptr<sac_stream> stream = std::make_unique<sac_stream>(seeder, testing_size*2);

    int i = 0;
    int difference = 0;
    std::vector<value_type> first_half;
    for (auto it : stream->next()) {

        if (i < testing_size) {
            first_half.push_back(it);
        }

        if (i >= testing_size && first_half.at((unsigned long) (i - testing_size)) != it) {
            difference++;
        }

        i++;
    }

    ASSERT_EQ(difference, 1); // The position of change is random, hence it is not necessary to check
}

TEST(sac_streams, fixed_position) {
    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unique_ptr<sac_fixed_pos_stream> stream = std::make_unique<sac_fixed_pos_stream>(seeder, testing_size*2, 9);

    int i = 0;
    int difference = 0;
    int difference_position = 0;
    int position_in_byte = 0;
    std::vector<value_type> first_half;
    for (auto it : stream->next()) {

        if (i < testing_size) {
            first_half.push_back(it);
        }

        if (i >= testing_size && first_half.at((unsigned long) (i - testing_size)) != it) {
            difference++;
            difference_position = i - testing_size;
            position_in_byte = first_half.at((unsigned long) (i - testing_size)) ^ it;
        }

        i++;
    }

    ASSERT_EQ(difference, 1); // There should be exactly one different byte between first and second half
    ASSERT_EQ(difference_position, 1); // Difference should be in second byte
    ASSERT_EQ(position_in_byte, 2); // Position x of changed bit, where position_in_byte = 2^x
}

TEST(column_streams, basic_test_with_counter) {
    json json_config = {
            {"size", 5},
            {"source", {
                {"type", "counter"}
                       }}
    };

    seed_seq_from<pcg32> seeder(testsuite::seed1);

    std::unique_ptr<column_stream> stream = std::make_unique<column_stream>(json_config, seeder, 5);

    // TODO: finish column test
//    for(auto it : stream->next()) {
//        std::cout << it << std::endl;
//    }
}













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

TEST(hw_counter, invalid_params) {
    const json json_config = {
        {"randomize_start", false},
        {"increase_hw", false},
        {"hw", 4*8+1},
    };

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::make_unique<hw_counter>(json_config, seeder, 5);
    EXPECT_THROW(std::make_unique<hw_counter>(json_config, seeder, 4), std::runtime_error);
}

TEST(hw_counter, basic_test) {
    const int hw = 3;
    const size_t size = 4;
    const int c_32_over_3 = 4960;

    const json json_config = {
        {"randomize_start", false},
        {"increase_hw", false},
        {"hw", hw},
    };

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    auto stream = std::make_unique<hw_counter>(json_config, seeder, size);

    // Test difference to previous vector
    uint8_t buff1[size], buff2[size];
    uint8_t *buff_o=buff1, *buff_n=buff2;

    for(int i=0; i < c_32_over_3 + 4; ++i){
        uint32_t cur_hw = 0;
        uint32_t idx = 0;

        for(auto it : stream->next()) {
            for(int j=0; j<8; ++j){
                cur_hw += (uint8_t)it & (1 << j) ? 1 : 0;
            }
            buff_n[idx] = (uint8_t)it;
            idx += 1;
        }

        ASSERT_EQ(cur_hw, hw);
        ASSERT_TRUE(i == 0 || memcmp(buff_o, buff_n, size) != 0);
        std::swap(buff_o, buff_n);
    }
}

TEST(hw_counter, period_test) {
    const int hw = 3;
    const size_t size = 4;
    const int c_32_over_3 = 4960;

    const json json_config = {
        {"randomize_start", false},
        {"increase_hw", false},
        {"hw", hw},
    };

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    auto stream1 = std::make_unique<hw_counter>(json_config, seeder, size);
    auto stream2 = std::make_unique<hw_counter>(json_config, seeder, size);

    // Rewind one stream beyond the period
    for(int i=0; i < c_32_over_3; ++i){
        stream1->next();
    }

    // Compare rewinded & fresh generator over period.
    for(int i=0; i < c_32_over_3 + 4; ++i){
        auto it1 = stream1->next();
        auto it2 = stream2->next();
        ASSERT_TRUE(it1 == it2);
    }

    // desync test
    stream1->next();
    for(int i=0; i < c_32_over_3 + 4; ++i){
        auto it1 = stream1->next();
        auto it2 = stream2->next();
        ASSERT_FALSE(it1 == it2);
    }
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

TEST(rnd_plt_ctx_streams, aes_single_vector) {
    const json json_config = {
        {"type", "rnd-plt-ctx-stream"},
        {"source", {
             {"type", "block"},
             {"init-frequency", "only-once"},
             {"algorithm", "AES"},
             {"round", 10},
             {"block-size", 16},
             {"plaintext", {{"type", "pcg32-stream"}}},
             {"key-size", 16},
             {"key", {{"type", "pcg32-stream"}}},
             {"iv", {{"type", "pcg32-stream"}}}
         }
        }
    };

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unique_ptr<stream> stream = make_stream(json_config, seeder, 32);

    vec_cview view = stream->next();

    // fixed expected data AES 10 rounds, with PTX choosen by PCG32 with fixed seed testsuite::seed1
#if defined(__clang__) // if compiler is clang, data are different from gcc :(
    std::vector<value_type> expected_data = {
        0x02, 0x6c, 0xc7, 0x6f, 0xaf, 0x91, 0x91, 0xb4, 0x82, 0x4c, 0x67, 0x8c, 0x22, 0x66, 0x39, 0x90,  // ptx
        0xd5, 0x36, 0xd3, 0x30, 0xf5, 0xe5, 0xde, 0x87, 0x75, 0x54, 0x82, 0x19, 0xf2, 0xb9, 0xa6, 0xcc   // ctx
    };
#elif defined(__GNUC__) || defined(__GNUG__)
    std::vector<value_type> expected_data = {
        0xc5, 0x1e, 0xf0, 0xd7, 0x40, 0x1c, 0xa5, 0xea, 0x6f, 0x85, 0x11, 0x1e, 0x55, 0x74, 0x64, 0xcc,  // ptx
        0x54, 0x30, 0x76, 0x5f, 0xbf, 0xa7, 0x0d, 0xcb, 0xf0, 0x9b, 0xb3, 0xc0, 0xbe, 0x2b, 0xba, 0xaf   // ctx
    };
#elif defined(_MSC_VER) // MS data unknown
    std::vector<value_type> expected_data = {
        // TODO
    };
#endif

    ASSERT_EQ(make_cview(expected_data), view);
}













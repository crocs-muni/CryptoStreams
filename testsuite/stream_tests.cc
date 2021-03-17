//
// Created by mhajas on 5/4/17.
//

#include "stream.h"
#include "streams.h"
#include "gtest/gtest.h"
#include <eacirc-core/seed.h>
#include <testsuite/test_utils/test_case.h>

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
    std::unique_ptr<counter> cstream = std::make_unique<counter>(testing_size / 256);

    for (int j = 1; j <= 65536 + 2; j++) { // 2^16 + 2
        auto data = cstream->next();
        std::vector<value_type> non_const(data.begin(), data.end());
        auto t = reinterpret_cast<uint32_t *>(non_const.data());

        ASSERT_EQ(j, *t);
    }
}

TEST(const_stream, basic_test) {
    {
        const json json_config = {{"value", "aA"}}; // shall pass
        std::make_unique<const_stream>(json_config, 1);
    }
    {
        const json json_config = {{"value", ""}};
        EXPECT_THROW(std::make_unique<const_stream>(json_config, 4), std::runtime_error);
    }
    {
        const json json_config = {{"value", "1122334"}}; // odd length
        EXPECT_THROW(std::make_unique<const_stream>(json_config, 4), std::runtime_error);
    }
    {
        const json json_config = {{"value", "1122334g"}}; // bad charset
        EXPECT_THROW(std::make_unique<const_stream>(json_config, 4), std::runtime_error);
    }
    {
        const json json_config = {{"value", "112233"}}; // invalid length
        EXPECT_THROW(std::make_unique<const_stream>(json_config, 4), std::runtime_error);
    }

    const json json_config = {{"value", "dEAdc0d3"}}; // OK, combined case
    auto stream = std::make_unique<const_stream>(json_config, 4);
    for (int i = 0; i < 5; ++i) {
        auto c = stream->next().copy_to_vector();
        ASSERT_EQ(c[0], 0xde);
        ASSERT_EQ(c[1], 0xad);
        ASSERT_EQ(c[2], 0xc0);
        ASSERT_EQ(c[3], 0xd3);
    }
}

TEST(sac_streams, basic_test) {
    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unique_ptr<sac_stream> stream = std::make_unique<sac_stream>(seeder, 16);

    for (unsigned i = 0; i < 16; ++i) {
        int difference = 0;
        auto first = stream->next();
        std::vector<value_type> first_half(first.begin(), first.end());
        auto second = stream->next();
        std::vector<value_type> second_half(second.begin(), second.end());

        auto first_beg = first_half.begin();
        auto second_beg = second_half.begin();
        for (; first_beg != first_half.end(); ++first_beg, ++second_beg) {
            if (*first_beg ^ *second_beg) {
                value_type diff = *first_beg ^ *second_beg;
                while (diff) {
                    if (diff & 0x01) {
                        ++difference;
                    }
                    diff >>= 1;
                }
            }
        }
        ASSERT_EQ(difference,
                  1); // The position of change is random, hence it is not necessary to check
    }
}

TEST(sac_streams, fixed_position) {
    seed_seq_from<pcg32> seeder(testsuite::seed1);
    auto stream = std::make_unique<sac_fixed_pos_stream>(
        seeder, 16, 10); // position 10 = second byte, third bit

    for (unsigned i = 0; i < 16; ++i) {
        int difference = 0;
        auto first = stream->next();
        std::vector<value_type> first_half(first.begin(), first.end());
        auto second = stream->next();
        std::vector<value_type> second_half(second.begin(), second.end());

        auto first_beg = first_half.begin();
        auto second_beg = second_half.begin();
        for (unsigned j = 0; first_beg != first_half.end(); ++first_beg, ++second_beg, ++j) {
            if (*first_beg ^ *second_beg) {
                ASSERT_EQ(j, 1); // Difference should be in second byte for given stream
                value_type diff = *first_beg ^ *second_beg;
                unsigned k = 0;
                while (diff) {
                    if (diff & 0x01) {
                        ASSERT_EQ(k, 2); // Difference should be in third bit for given stream
                        ++difference;
                    }
                    diff >>= 1;
                    ++k;
                }
            }
        }
        ASSERT_EQ(difference,
                  1); // There should be exactly one different byte between first and second half
    }
}

TEST(hw_counter, invalid_params) {
    const json json_config = {
        {"randomize_start", false},
        {"increase_hw", false},
        {"hw", 4 * 8 + 1},
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
    uint8_t *buff_o = buff1, *buff_n = buff2;

    for (int i = 0; i < c_32_over_3 + 4; ++i) {
        uint32_t cur_hw = 0;
        uint32_t idx = 0;

        for (auto it : stream->next()) {
            for (int j = 0; j < 8; ++j) {
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
    for (int i = 0; i < c_32_over_3; ++i) {
        stream1->next();
    }

    // Compare rewinded & fresh generator over period.
    for (int i = 0; i < c_32_over_3 + 4; ++i) {
        auto it1 = stream1->next();
        auto it2 = stream2->next();
        ASSERT_TRUE(it1 == it2);
    }

    // desync test
    stream1->next();
    for (int i = 0; i < c_32_over_3 + 4; ++i) {
        auto it1 = stream1->next();
        auto it2 = stream2->next();
        ASSERT_FALSE(it1 == it2);
    }
}

TEST(hw_counter, init_state_test) {
    seed_seq_from<pcg32> seeder(testsuite::seed1);

    { // OK
        const json json_config = {
            {"increase_hw", false}, {"hw", 4}, {"initial_state", {0, 1, 2, 3}}};
        std::make_unique<hw_counter>(json_config, seeder, 8);
    }
    { // invalid count
        const json json_config = {{"increase_hw", false}, {"hw", 2}, {"initial_state", {0}}};
        EXPECT_THROW(std::make_unique<hw_counter>(json_config, seeder, 8), std::runtime_error);
    }
    { // out of bounds
        const json json_config = {{"increase_hw", false}, {"hw", 2}, {"initial_state", {0, 8 * 8}}};
        EXPECT_THROW(std::make_unique<hw_counter>(json_config, seeder, 8), std::runtime_error);
    }
    { // sequence
        const json json_config = {
            {"increase_hw", false}, {"hw", 4}, {"initial_state", {0, 1, 1, 2}}};
        EXPECT_THROW(std::make_unique<hw_counter>(json_config, seeder, 8), std::runtime_error);
    }

    const json json_config = {
        {"increase_hw", false}, {"hw", 4}, {"initial_state", {0, 13, 14, 15}}};
    auto stream = std::make_unique<hw_counter>(json_config, seeder, 2); // 16 bits
    auto c1 = stream->next().copy_to_vector();
    ASSERT_EQ(c1[0], 0x01);
    ASSERT_EQ(c1[1], 0xe0);

    auto c2 = stream->next().copy_to_vector();
    ASSERT_EQ(c2[0], 0x1e);
    ASSERT_EQ(c2[1], 0x00);
}

TEST(column_streams, test_with_counter) {
    json json_config = R"({
       "type": "column_stream",
       "size": 16,
       "source": {
           "type": "block",
           "init_frequency": "only_once",
           "algorithm": "AES",
           "round": 10,
           "block_size": 16,
           "plaintext": {
               "type": "counter"
           },
           "key_size": 16,
           "key": {
               "type": "false_stream"
           },
           "iv": {
               "type": "false_stream"
           }
       }
    })"_json;

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> map;

    std::unique_ptr<column_stream> stream =
        std::make_unique<column_stream>(json_config, seeder, map, 16);

    // first two bytes of each input vector for computed in Python expected output
    std::vector<std::vector<value_type>> expected = {
        {0x52, 0x8f}, {0xaf, 0x95}, {0x5b, 0x93}, {0x5a, 0xa5}, {0x76, 0x20}, {0xef, 0xa3},
        {0xbe, 0x5e}, {0xac, 0x15}, {0x77, 0xea}, {0xe1, 0x7f}, {0xf1, 0xbe}, {0xea, 0xbf},
        {0x31, 0x78}, {0x30, 0x6d}, {0x04, 0x13}, {0xc8, 0xce}, {0x06, 0x75}, {0x66, 0xbc},
        {0x6f, 0x80}, {0xd8, 0xd1}, {0xad, 0xad}, {0x4a, 0xcc}, {0x57, 0x9f}, {0x23, 0xb3},
        {0x56, 0x02}, {0x33, 0x6c}, {0x7a, 0xdd}, {0xb9, 0xe3}, {0x18, 0x0b}, {0xee, 0x9a},
        {0xfa, 0xcb}, {0x72, 0xf8}, {0xc3, 0x8d}, {0xd7, 0x2e}, {0xf5, 0xef}, {0x02, 0xaf},
        {0xe4, 0xf4}, {0x2d, 0xd5}, {0x6e, 0xb1}, {0x95, 0x20}, {0x62, 0x23}, {0x26, 0x26},
        {0x56, 0xcb}, {0xba, 0x60}, {0xde, 0x44}, {0xea, 0x19}, {0x1a, 0x27}, {0xeb, 0x9a},
        {0x67, 0xc7}, {0x91, 0xa9}, {0xbb, 0xd2}, {0x13, 0x5f}, {0x93, 0x72}, {0x94, 0xfe},
        {0xa3, 0x2e}, {0xb3, 0xf1}, {0xd7, 0x74}, {0x9b, 0x9c}, {0xb2, 0x3c}, {0xa3, 0x57},
        {0x06, 0xf5}, {0x67, 0xf7}, {0x27, 0xd1}, {0x5f, 0x44}, {0x6b, 0x65}, {0xeb, 0xdd},
        {0x64, 0x17}, {0xb5, 0xbb}, {0xc1, 0xff}, {0x3d, 0x5d}, {0x44, 0xba}, {0xc9, 0x0b},
        {0xc0, 0x0f}, {0x3b, 0x19}, {0xe2, 0x87}, {0x9e, 0x68}, {0xc8, 0x08}, {0x40, 0x94},
        {0x82, 0x9a}, {0xa1, 0x0d}, {0xc2, 0xab}, {0x66, 0x7e}, {0xde, 0x25}, {0xb7, 0xc9},
        {0xec, 0xdf}, {0x95, 0x03}, {0xf1, 0xe7}, {0xbc, 0xc9}, {0x0d, 0x0c}, {0x38, 0x8a},
        {0xc2, 0x44}, {0x59, 0x26}, {0x92, 0xbf}, {0x72, 0x7a}, {0xc5, 0xe1}, {0xa6, 0xee},
        {0x86, 0x32}, {0x9c, 0xc5}, {0xde, 0x0f}, {0x99, 0xc4}, {0x02, 0x00}, {0xd6, 0xdb},
        {0x2c, 0x94}, {0xa7, 0x4a}, {0xaa, 0xb9}, {0x67, 0x41}, {0x48, 0x48}, {0x0f, 0xe2},
        {0x8b, 0xde}, {0x88, 0x31}, {0xf2, 0x15}, {0x3f, 0x24}, {0x4b, 0xf7}, {0x3a, 0xe4},
        {0x41, 0xb2}, {0x01, 0x86}, {0xa1, 0x3a}, {0x8a, 0x13}, {0xc2, 0x7a}, {0x84, 0x85},
        {0xce, 0xec}, {0x9a, 0x33}, {0x05, 0xb6}, {0xa3, 0x1c}, {0x01, 0x61}, {0x2d, 0x66},
        {0x83, 0xe9}, {0xd0, 0x2e}};

    // regressive tests: computed results from this implementation:
    std::vector<std::vector<value_type>> precomputed = {
        {0x52, 0x8f, 0x57, 0x4, 0xf6, 0x43, 0x50, 0xbe, 0x86, 0xdd, 0x88, 0x3d, 0x5e, 0xd, 0x76, 0xc2},
        {0xaf, 0x95, 0x4, 0x52, 0xc1, 0x80, 0x91, 0x95, 0xaf, 0xd, 0xe4, 0x73, 0x16, 0xc2, 0x9c, 0xe8},
        {0x5b, 0x93, 0xce, 0xa0, 0x26, 0x83, 0xe3, 0x4, 0x52, 0x95, 0xbc, 0x13, 0xc4, 0x7, 0xfb, 0xe5},
        {0x5a, 0xa5, 0xe7, 0x9f, 0xe7, 0xd, 0x84, 0x12, 0x99, 0xd8, 0xf1, 0x14, 0x34, 0xb7, 0x5a, 0x9d},
        {0x76, 0x20, 0xe2, 0x68, 0xac, 0x7, 0xb7, 0x84, 0xd7, 0xd, 0xa2, 0xfb, 0x82, 0xe, 0x16, 0x6f},
        {0xef, 0xa3, 0x6, 0x97, 0x52, 0x7c, 0xfa, 0xf5, 0xfc, 0x51, 0x42, 0x5a, 0xf0, 0xf3, 0x2b, 0xac},
        {0xbe, 0x5e, 0x95, 0x49, 0xac, 0x35, 0xa4, 0x2, 0x88, 0x27, 0xed, 0x94, 0xc7, 0xbf, 0xdb, 0xc5},
        {0xac, 0x15, 0x4e, 0x40, 0xdf, 0x0, 0x16, 0x4d, 0x72, 0x8e, 0xa1, 0xab, 0x66, 0xfd, 0x2a, 0xca},
        {0x77, 0xea, 0xc4, 0xd8, 0xec, 0x8e, 0x71, 0xf, 0xaa, 0xa, 0x2b, 0x25, 0xc4, 0xbd, 0x22, 0xf7},
        {0xe1, 0x7f, 0x13, 0x22, 0xbc, 0x3c, 0x1b, 0x38, 0xb4, 0x86, 0xc3, 0x34, 0xdc, 0x1f, 0xef, 0x21},
        {0xf1, 0xbe, 0xf0, 0x1c, 0x1, 0x3d, 0x8c, 0x3d, 0x37, 0xde, 0x26, 0xb2, 0x97, 0x38, 0x65, 0xbc},
        {0xea, 0xbf, 0xf8, 0xc1, 0x57, 0x7b, 0x45, 0x8b, 0x28, 0x74, 0x4a, 0x83, 0xbb, 0x98, 0xf8, 0x1d},
        {0x31, 0x78, 0x13, 0x9c, 0x2, 0x48, 0x65, 0x31, 0x7, 0x36, 0xbd, 0x8a, 0x51, 0x44, 0xd7, 0x5e},
        {0x30, 0x6d, 0x9a, 0x37, 0xac, 0xda, 0x6, 0x41, 0x86, 0x13, 0x98, 0x9a, 0x57, 0x55, 0x33, 0x71},
        {0x4, 0x13, 0xb8, 0x7e, 0x21, 0xcc, 0x1c, 0x78, 0xa9, 0xc3, 0x61, 0x7, 0xe3, 0xe8, 0x74, 0x71},
        {0xc8, 0xce, 0x36, 0x3f, 0xa8, 0xbe, 0xd1, 0xf4, 0x69, 0xdc, 0x0, 0x2b, 0xd0, 0xa6, 0x61, 0x65},
        {0x6, 0x75, 0x4f, 0xf2, 0xcb, 0x9, 0xe1, 0xaa, 0x1a, 0x5e, 0xe, 0x19, 0x7e, 0xfd, 0x2a, 0x51},
        {0x66, 0xbc, 0x76, 0x17, 0x5f, 0x6c, 0xdb, 0x7, 0xe6, 0x77, 0xb9, 0x1f, 0xc1, 0xc2, 0x95, 0x48},
        {0x6f, 0x80, 0x7c, 0x9c, 0xe2, 0x6d, 0x70, 0xd7, 0x27, 0x28, 0xc2, 0xca, 0x5d, 0x17, 0x4f, 0x92},
        {0xd8, 0xd1, 0x7d, 0x15, 0xf9, 0xae, 0x45, 0xa7, 0x85, 0x6a, 0xee, 0x8, 0x26, 0x66, 0xd3, 0x4c},
        {0xad, 0xad, 0x41, 0x6c, 0xda, 0xba, 0x5c, 0x16, 0xaf, 0x6b, 0xe6, 0xd5, 0xc3, 0xa8, 0x5b, 0xfb},
        {0x4a, 0xcc, 0x86, 0x83, 0x90, 0x95, 0xa7, 0x66, 0x5, 0xd, 0x82, 0x97, 0x6a, 0x26, 0xd0, 0xd9},
        {0x57, 0x9f, 0xbb, 0x77, 0xd5, 0x87, 0x7c, 0xfe, 0x3d, 0x62, 0x28, 0x59, 0x93, 0x5a, 0x1, 0xcb},
        {0x23, 0xb3, 0xa7, 0xc8, 0xbc, 0xda, 0xad, 0xa0, 0x63, 0x53, 0x8b, 0x58, 0xb9, 0x6c, 0x73, 0x1c},
        {0x56, 0x2, 0x14, 0xcd, 0x9a, 0xbe, 0x94, 0xa6, 0x17, 0x88, 0x9a, 0x65, 0xda, 0x38, 0x19, 0xe2},
        {0x33, 0x6c, 0x5c, 0x12, 0xc7, 0x35, 0xef, 0xc3, 0x6b, 0x27, 0x9d, 0x92, 0x82, 0x58, 0x7f, 0x31},
        {0x7a, 0xdd, 0xb4, 0x12, 0x95, 0x7d, 0x77, 0x3b, 0xf4, 0x46, 0xa8, 0x78, 0x10, 0x72, 0x55, 0x7},
        {0xb9, 0xe3, 0x95, 0x44, 0xe5, 0x41, 0x92, 0xf3, 0x78, 0x6a, 0x13, 0xf9, 0x1e, 0x9a, 0x8e, 0x35},
        {0x18, 0xb, 0x9, 0x2e, 0xd7, 0xf5, 0x1f, 0xeb, 0x69, 0x95, 0xd0, 0x5b, 0xd2, 0xcc, 0x43, 0xf6},
        {0xee, 0x9a, 0xce, 0x3a, 0xfc, 0x14, 0xcc, 0x4f, 0xdd, 0xc8, 0xaf, 0x9f, 0x6f, 0xc0, 0x65, 0xf0},
        {0xfa, 0xcb, 0xa1, 0x87, 0x31, 0xbe, 0x2b, 0xaa, 0x54, 0x2b, 0x83, 0x53, 0xb4, 0x83, 0xc1, 0xab},
        {0x72, 0xf8, 0x3f, 0x25, 0xcf, 0x87, 0x65, 0xa1, 0x96, 0x3a, 0x6f, 0xb8, 0x47, 0xd3, 0xd2, 0x84},
        {0xc3, 0x8d, 0xcf, 0xab, 0xbc, 0x22, 0x8f, 0xd6, 0xa2, 0xe2, 0x99, 0x7d, 0x49, 0x71, 0x6e, 0x28},
        {0xd7, 0x2e, 0x87, 0xcc, 0x13, 0x7d, 0x44, 0x48, 0x94, 0x52, 0x4, 0xa7, 0x65, 0x54, 0x93, 0xed},
        {0xf5, 0xef, 0xd2, 0x78, 0x58, 0xaf, 0x91, 0x19, 0xfe, 0xcf, 0x45, 0xed, 0xd4, 0x22, 0x67, 0x39},
        {0x2, 0xaf, 0xcf, 0x74, 0x0, 0x3c, 0x74, 0x54, 0x7c, 0x2a, 0x73, 0xdc, 0x4, 0x94, 0x74, 0xfa},
        {0xe4, 0xf4, 0xcb, 0xf0, 0xe2, 0xf9, 0xc3, 0xb7, 0xbd, 0x9d, 0xf, 0xe4, 0x55, 0x6b, 0x8d, 0x3b},
        {0x2d, 0xd5, 0x44, 0xe2, 0xc8, 0x65, 0x17, 0x54, 0x34, 0x62, 0xe7, 0x82, 0xf3, 0x55, 0x57, 0x81},
        {0x6e, 0xb1, 0x22, 0xcc, 0x4d, 0xa3, 0xe0, 0x60, 0x12, 0xb, 0x95, 0xd5, 0xb1, 0x6b, 0xc1, 0x9e},
        {0x95, 0x20, 0xbf, 0xd8, 0xad, 0xa6, 0xd9, 0xad, 0x1b, 0xb3, 0x75, 0xc0, 0x5f, 0xd3, 0x46, 0xa8},
        {0x62, 0x23, 0xf8, 0xbe, 0x75, 0xb6, 0xe3, 0x59, 0xb8, 0x2c, 0x53, 0x26, 0x9, 0x59, 0xc3, 0xd6},
        {0x26, 0x26, 0xe2, 0x3c, 0x0, 0x5e, 0xea, 0xb9, 0x63, 0xe3, 0xf1, 0x33, 0x34, 0xd3, 0xc, 0x4a},
        {0x56, 0xcb, 0x39, 0x7a, 0x49, 0x3b, 0xa6, 0x35, 0x6a, 0x97, 0xb8, 0x67, 0x4f, 0xb2, 0x69, 0x6c},
        {0xba, 0x60, 0x73, 0x5d, 0xdf, 0x92, 0xaa, 0xcc, 0x87, 0x7, 0x61, 0xf4, 0x76, 0xd1, 0x38, 0xdb},
        {0xde, 0x44, 0xf1, 0xe1, 0x1b, 0x5f, 0xa2, 0xa9, 0xf4, 0x11, 0x34, 0x61, 0xa9, 0x30, 0xbd, 0xb3},
        {0xea, 0x19, 0x88, 0x88, 0xd, 0x52, 0xfe, 0xc6, 0x5c, 0xbd, 0x66, 0x35, 0x22, 0x42, 0x9c, 0x71},
        {0x1a, 0x27, 0x7f, 0x8, 0x6b, 0x87, 0xbb, 0x84, 0x1c, 0x3e, 0xd5, 0xa3, 0x90, 0x14, 0x5d, 0x1b},
        {0xeb, 0x9a, 0xda, 0x56, 0x20, 0xf6, 0x9e, 0x2e, 0x4, 0xb2, 0x2, 0x3b, 0x1, 0xa4, 0x54, 0xea},
        {0x67, 0xc7, 0x69, 0x43, 0x8d, 0xd, 0x9, 0xae, 0x1a, 0x25, 0x8c, 0x2a, 0xc9, 0x6b, 0x4b, 0x2f},
        {0x91, 0xa9, 0x4e, 0x55, 0x44, 0x1f, 0xf7, 0x80, 0xb3, 0x77, 0xf6, 0x28, 0xb4, 0x71, 0xe5, 0xe5},
        {0xbb, 0xd2, 0xc3, 0xc7, 0x13, 0x2f, 0xbc, 0xf7, 0x36, 0x89, 0x53, 0xf6, 0xcf, 0x6d, 0x47, 0x6},
        {0x13, 0x5f, 0x68, 0x57, 0x58, 0xe1, 0xb5, 0x74, 0xbc, 0xf8, 0xbe, 0x81, 0x2d, 0x32, 0xfb, 0x64},
        {0x93, 0x72, 0x4d, 0x1a, 0xa2, 0x7f, 0xb, 0xee, 0xe2, 0x5c, 0xc3, 0xaf, 0x65, 0xc5, 0x17, 0xb4},
        {0x94, 0xfe, 0x14, 0x4d, 0xd6, 0xcb, 0x5a, 0x90, 0x71, 0x1c, 0xf4, 0x1, 0xb2, 0xf0, 0x18, 0x78},
        {0xa3, 0x2e, 0x4d, 0x6, 0xd7, 0xc2, 0xdd, 0xe0, 0x25, 0xea, 0x69, 0xc0, 0x41, 0xaf, 0x77, 0x6e},
        {0xb3, 0xf1, 0x44, 0x15, 0x4f, 0x5f, 0xdf, 0x4a, 0xbf, 0xfc, 0x24, 0xcc, 0xb3, 0x1d, 0x5e, 0x66},
        {0xd7, 0x74, 0xfb, 0xc6, 0x61, 0xf8, 0x4c, 0xf6, 0xaf, 0x47, 0x1a, 0x44, 0xd4, 0xca, 0xf3, 0x3c},
        {0x9b, 0x9c, 0x2a, 0x9c, 0x9e, 0xfa, 0x26, 0xbd, 0x5f, 0x78, 0x9f, 0xab, 0x23, 0xaf, 0x6a, 0x28},
        {0xb2, 0x3c, 0xca, 0x37, 0x9, 0xe5, 0x57, 0xa8, 0x55, 0x4b, 0xd5, 0x52, 0x9, 0xca, 0x80, 0x53},
        {0xa3, 0x57, 0x62, 0x2a, 0xfa, 0x56, 0xb9, 0xda, 0x45, 0x6, 0x12, 0xf9, 0xf1, 0xca, 0x9c, 0xc8},
        {0x6, 0xf5, 0x37, 0xdf, 0xf0, 0xb4, 0x37, 0xc7, 0x1e, 0xc1, 0xb9, 0xaf, 0xe0, 0x54, 0xd3, 0xa3},
        {0x67, 0xf7, 0x6f, 0x3c, 0xd4, 0xa3, 0x2a, 0x7f, 0x42, 0x62, 0x52, 0xa9, 0x1, 0x2f, 0xca, 0x8e},
        {0x27, 0xd1, 0xd6, 0xc2, 0x82, 0xce, 0x9b, 0xfe, 0x8e, 0x6, 0x1f, 0x95, 0x4e, 0x44, 0xa4, 0x87},
        {0x5f, 0x44, 0xf, 0xc4, 0x10, 0xed, 0xf, 0x83, 0x83, 0xdf, 0x79, 0xa8, 0xf, 0xda, 0x13, 0xbd},
        {0x6b, 0x65, 0xa2, 0xc6, 0xe0, 0x99, 0xe2, 0x91, 0x13, 0x88, 0xa0, 0xbd, 0xb2, 0xac, 0x5, 0xbe},
        {0xeb, 0xdd, 0xc2, 0x40, 0x4d, 0xad, 0x3b, 0xae, 0x3f, 0xc5, 0x9f, 0x60, 0x94, 0xa2, 0x76, 0xef},
        {0x64, 0x17, 0x24, 0x4, 0x37, 0x58, 0x1, 0xa4, 0xd4, 0xfd, 0xb9, 0x5f, 0x24, 0x43, 0x54, 0x87},
        {0xb5, 0xbb, 0x49, 0xae, 0xed, 0x2d, 0xf4, 0x86, 0x13, 0xe7, 0xee, 0xb8, 0x5, 0x76, 0xbe, 0x87},
        {0xc1, 0xff, 0xbe, 0x54, 0x78, 0x8a, 0x3, 0xb7, 0x61, 0x93, 0x67, 0x5a, 0x26, 0xd8, 0xd2, 0x23},
        {0x3d, 0x5d, 0xa1, 0xbb, 0xbe, 0x2, 0xb8, 0x7a, 0xf2, 0xe8, 0x4c, 0x3b, 0x73, 0xd2, 0x15, 0xf5},
        {0x44, 0xba, 0xc9, 0x70, 0xba, 0xf6, 0xa8, 0xb8, 0x8c, 0x6d, 0xdd, 0xf9, 0xc2, 0xa2, 0xa0, 0xbb},
        {0xc9, 0xb, 0xcd, 0x23, 0x4f, 0x63, 0x13, 0x44, 0xb5, 0xdd, 0x61, 0x28, 0x18, 0x96, 0xff, 0x8e},
        {0xc0, 0xf, 0xa6, 0x19, 0x19, 0x47, 0xb0, 0xf5, 0x99, 0x39, 0x99, 0xbd, 0xe1, 0x67, 0x4a, 0x95},
        {0x3b, 0x19, 0xe8, 0x23, 0x91, 0xbd, 0x73, 0xa9, 0xd2, 0x34, 0xe2, 0x9a, 0x21, 0x3b, 0x33, 0x16},
        {0xe2, 0x87, 0x8c, 0x6c, 0xe5, 0xe8, 0x9f, 0x66, 0x6, 0xda, 0xb, 0xf1, 0x16, 0x9b, 0x32, 0x99},
        {0x9e, 0x68, 0x7f, 0x8, 0x7, 0xbc, 0xc0, 0x1d, 0xff, 0xf, 0xf1, 0x79, 0x4, 0xc5, 0x82, 0xaf},
        {0xc8, 0x8, 0x57, 0x95, 0x62, 0xee, 0x7b, 0xb, 0x2d, 0xf0, 0xe5, 0x4, 0x93, 0x66, 0x44, 0x9b},
        {0x40, 0x94, 0x71, 0x89, 0x6c, 0x6a, 0x7, 0x31, 0x37, 0xf8, 0xbb, 0x30, 0xf6, 0xa3, 0x30, 0xdb},
        {0x82, 0x9a, 0x1c, 0xd6, 0xea, 0xeb, 0x79, 0xef, 0x3, 0x9b, 0xe4, 0xba, 0x2, 0xe, 0x8e, 0xed},
        {0xa1, 0xd, 0x8d, 0xbd, 0x63, 0x1a, 0x72, 0x8b, 0xb8, 0x5, 0x36, 0xab, 0xe9, 0xc1, 0x1e, 0x7b},
        {0xc2, 0xab, 0xad, 0x89, 0xc8, 0x5a, 0xb2, 0x43, 0x92, 0x82, 0x8b, 0xba, 0xf5, 0x3d, 0xa, 0xe5},
        {0x66, 0x7e, 0x92, 0x78, 0x8a, 0x1d, 0x78, 0x64, 0x4c, 0x77, 0x5d, 0x35, 0x29, 0xf5, 0xe2, 0xf3},
        {0xde, 0x25, 0xaa, 0xa1, 0x20, 0x90, 0x2c, 0xd, 0xfd, 0xb1, 0xdf, 0xc0, 0x7f, 0x7b, 0xb8, 0xc1},
        {0xb7, 0xc9, 0xeb, 0x2b, 0x86, 0xfd, 0xdb, 0x8b, 0xd2, 0x51, 0x51, 0xed, 0xd8, 0xe8, 0xc9, 0xee},
        {0xec, 0xdf, 0x7a, 0x7d, 0x6, 0x17, 0xee, 0x99, 0xbc, 0x7c, 0xe5, 0x4d, 0xfd, 0xbc, 0x4d, 0xab},
        {0x95, 0x3, 0x18, 0xd3, 0xd, 0x16, 0xbd, 0xb2, 0x62, 0xea, 0x64, 0x93, 0x14, 0x1b, 0x5f, 0x46},
        {0xf1, 0xe7, 0xcb, 0x4, 0xc1, 0x3f, 0x2c, 0xe1, 0x1, 0x75, 0x31, 0x6d, 0xdc, 0x51, 0xb3, 0x5e},
        {0xbc, 0xc9, 0x3e, 0x8f, 0xd6, 0x76, 0xbe, 0xdc, 0x4f, 0x89, 0x7b, 0xf3, 0x92, 0x6d, 0xe, 0xc1},
        {0xd, 0xc, 0x66, 0x16, 0x3, 0x6a, 0x4c, 0xc, 0x95, 0x67, 0x6f, 0xdb, 0xb6, 0x8, 0xd6, 0x10},
        {0x38, 0x8a, 0x8b, 0x7d, 0xe4, 0xb2, 0xb9, 0x20, 0x98, 0x5c, 0x60, 0x6, 0xe8, 0x5d, 0x72, 0xe6},
        {0xc2, 0x44, 0x33, 0x5d, 0x2f, 0x32, 0x3e, 0xf9, 0x4f, 0xc8, 0xb1, 0x4c, 0x25, 0x1e, 0xd5, 0x27},
        {0x59, 0x26, 0x32, 0x70, 0xeb, 0x3c, 0x93, 0xbe, 0xb0, 0x6, 0xbf, 0x96, 0x49, 0x1e, 0x23, 0x9c},
        {0x92, 0xbf, 0xb2, 0x68, 0x5d, 0x4c, 0xfe, 0x5d, 0x31, 0x16, 0x13, 0x23, 0x2e, 0x9c, 0x7f, 0xcf},
        {0x72, 0x7a, 0x1f, 0xc8, 0xdd, 0xf7, 0xf, 0xa1, 0x4f, 0xa6, 0x9b, 0xd3, 0x5e, 0x9c, 0x5c, 0x18},
        {0xc5, 0xe1, 0x6f, 0xad, 0x81, 0xd7, 0x58, 0xf3, 0xa1, 0x4f, 0x58, 0x92, 0x58, 0xba, 0xb7, 0xb9},
        {0xa6, 0xee, 0x65, 0xf8, 0x23, 0xde, 0x8a, 0x57, 0x54, 0xa9, 0x21, 0x17, 0xd9, 0x14, 0xa4, 0x97},
        {0x86, 0x32, 0x78, 0xe, 0x55, 0x4c, 0xe6, 0xb8, 0x7c, 0xf1, 0x6b, 0x57, 0xc3, 0x5c, 0xa1, 0xc6},
        {0x9c, 0xc5, 0x45, 0x24, 0xd, 0x87, 0x79, 0x24, 0x23, 0x43, 0xed, 0x1c, 0x9a, 0xde, 0x0, 0x9a},
        {0xde, 0xf, 0x81, 0x58, 0xaf, 0xf6, 0x5a, 0x9a, 0x77, 0x8a, 0xd1, 0x82, 0xd1, 0x21, 0xcd, 0x5b},
        {0x99, 0xc4, 0xaa, 0x6, 0x36, 0x1c, 0xb, 0xea, 0x3, 0x16, 0x9f, 0x45, 0xfa, 0x30, 0xb1, 0xa8},
        {0x2, 0x0, 0x2f, 0xb8, 0x86, 0x3, 0x30, 0xb7, 0x3b, 0xba, 0x4f, 0xfe, 0xde, 0xc6, 0xcd, 0xc8},
        {0xd6, 0xdb, 0xd0, 0xa6, 0xc0, 0x30, 0xdc, 0xe4, 0xf8, 0x6e, 0xa8, 0x68, 0xa7, 0x38, 0xcb, 0x50},
        {0x2c, 0x94, 0x10, 0xbd, 0xa, 0x29, 0x97, 0xf7, 0x57, 0x7c, 0xec, 0xa6, 0x42, 0x35, 0xb, 0x69},
        {0xa7, 0x4a, 0x17, 0x2, 0x9d, 0xce, 0x52, 0xf5, 0x2c, 0x6d, 0x33, 0x36, 0xd1, 0xa, 0x6f, 0xdd},
        {0xaa, 0xb9, 0xd1, 0xf4, 0xfe, 0x85, 0x89, 0xfe, 0x5f, 0xf5, 0xbf, 0x74, 0xc7, 0x2c, 0x5, 0x43},
        {0x67, 0x41, 0x2c, 0xb0, 0x37, 0xf9, 0xc8, 0x34, 0x8d, 0x97, 0x44, 0xd7, 0xb3, 0x91, 0xa0, 0x35},
        {0x48, 0x48, 0xe7, 0x24, 0xc3, 0xde, 0xa8, 0xaa, 0xa7, 0x1b, 0xbd, 0x59, 0x6f, 0x7, 0x7e, 0xa4},
        {0xf, 0xe2, 0xfa, 0x90, 0x39, 0x95, 0xf5, 0x42, 0x64, 0x40, 0xc6, 0x31, 0xf7, 0x8b, 0x2c, 0x9d},
        {0x8b, 0xde, 0x39, 0x36, 0xe, 0xe8, 0xad, 0x15, 0xe8, 0x8, 0xc3, 0x8d, 0x29, 0xf5, 0xdb, 0x93},
        {0x88, 0x31, 0xb4, 0xdc, 0x42, 0x61, 0x6b, 0xcf, 0x1e, 0x4c, 0xb0, 0xd8, 0xa7, 0x4d, 0xc, 0x8e},
        {0xf2, 0x15, 0xe4, 0x2c, 0x5b, 0xd7, 0xca, 0xa0, 0xd0, 0xeb, 0xa0, 0xd3, 0x4b, 0xd3, 0x17, 0x30},
        {0x3f, 0x24, 0x45, 0x1e, 0xfd, 0x13, 0xa3, 0xa8, 0x5d, 0x12, 0x3e, 0xdc, 0x30, 0xe9, 0x91, 0x59},
        {0x4b, 0xf7, 0x22, 0x48, 0x79, 0x69, 0xed, 0x97, 0xe9, 0x3e, 0xe6, 0x53, 0x5f, 0x1f, 0xfa, 0xbf},
        {0x3a, 0xe4, 0x1, 0xfe, 0x10, 0x9d, 0x6b, 0x32, 0xa1, 0x35, 0x15, 0x74, 0x5e, 0xdb, 0xf6, 0x49},
        {0x41, 0xb2, 0xb3, 0xc9, 0x89, 0x66, 0x3d, 0xf, 0x52, 0x74, 0x1c, 0xc8, 0x5e, 0xfe, 0xb3, 0xa7},
        {0x1, 0x86, 0x52, 0x38, 0x5f, 0xd9, 0xce, 0x10, 0xe3, 0xe8, 0xbe, 0x3a, 0x91, 0xc3, 0xd1, 0x54},
        {0xa1, 0x3a, 0x17, 0x81, 0x4, 0x6e, 0xf, 0xf9, 0x29, 0xe2, 0x89, 0xbf, 0x29, 0x5c, 0x83, 0x5f},
        {0x8a, 0x13, 0xb5, 0x79, 0x16, 0x8c, 0x4d, 0xf5, 0x20, 0xb3, 0x18, 0x98, 0xfa, 0x62, 0x68, 0x8b},
        {0xc2, 0x7a, 0x5a, 0xbb, 0x77, 0x18, 0x53, 0x1c, 0xef, 0x5, 0xfb, 0x81, 0x4f, 0xe, 0x8b, 0xe4},
        {0x84, 0x85, 0x9a, 0x7c, 0xb3, 0xfa, 0x2e, 0x39, 0x50, 0x91, 0xfa, 0x3d, 0xaa, 0xb, 0xf8, 0x44},
        {0xce, 0xec, 0x6a, 0x40, 0x8f, 0xc4, 0xaa, 0xf9, 0x6a, 0x37, 0x33, 0x10, 0x26, 0x96, 0x56, 0x1c},
        {0x9a, 0x33, 0x28, 0x3e, 0x8e, 0xc4, 0x24, 0x98, 0x3e, 0xab, 0x71, 0xda, 0xd8, 0x70, 0xec, 0xae},
        {0x5, 0xb6, 0xcb, 0xad, 0xe1, 0x5, 0x1e, 0x68, 0x7b, 0x24, 0x81, 0x9c, 0xbb, 0x33, 0xde, 0xfd},
        {0xa3, 0x1c, 0x97, 0xcd, 0xd3, 0x84, 0xc2, 0xad, 0xae, 0xd1, 0x1d, 0x59, 0x5c, 0x52, 0xbc, 0x8d},
        {0x1, 0x61, 0xae, 0xdd, 0xa1, 0x5, 0x3c, 0xdb, 0xea, 0xd5, 0x43, 0x25, 0xbe, 0x59, 0x58, 0x3a},
        {0x2d, 0x66, 0x8d, 0x51, 0xa, 0x74, 0x80, 0x3c, 0xcf, 0xe6, 0xcb, 0x51, 0x79, 0xb0, 0x5b, 0xcf},
        {0x83, 0xe9, 0x6d, 0x57, 0x30, 0x63, 0x41, 0x1f, 0x72, 0xc7, 0xbf, 0xa, 0xc6, 0xd8, 0x8d, 0x10},
        {0xd0, 0x2e, 0xac, 0x45, 0x92, 0xb7, 0xc1, 0x65, 0x1d, 0x1c, 0xad, 0x99, 0xb7, 0x58, 0xa6, 0x10}
    };

    for (unsigned i = 0; i < precomputed.size(); ++i) {
        vec_cview data = stream->next();

        // manualy precomputed are only limited number of values
        if (i < expected.size())
            ASSERT_TRUE(expected[i][0] == data.data()[0] and expected[i][1] == data.data()[1]);

        // but test all for regresive tests
        ASSERT_EQ(precomputed[i], data.copy_to_vector());

        // code used to generate reference data code:
        /*std::cout << "{";
        for (auto & val : data) {
            std::cout << "0x" << std::hex << unsigned(val) << ", ";
        }
        std::cout << "},\n";*/
    }


}

TEST(rnd_plt_ctx_streams, aes_single_vector) {
    const json json_config = R"({
         "type": "tuple_stream",
         "sources": [{
                 "type": "block",
                 "output_size": 16,
                 "init_frequency": "only_once",
                 "algorithm": "AES",
                 "round": 10,
                 "block_size": 16,
                 "plaintext": {
                     "type": "pipe_in_stream",
                     "id": "ptx_stream",
                     "source": {
                         "type": "pcg32_stream"
                     }
                 },
                 "key_size": 16,
                 "key": {
                     "type": "pcg32_stream"
                 },
                 "iv": {
                     "type": "pcg32_stream"
                 }
             },
             {
                 "type": "pipe_out_stream",
                 "id": "ptx_stream"
             }
         ]
     }
    )"_json;

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> map;

    std::unique_ptr<stream> stream = make_stream(json_config, seeder, map, 32);

    vec_cview view = stream->next();

    // fixed expected data AES 10 rounds, with PTX chosen by PCG32 with fixed seed testsuite::seed1
    std::vector<value_type> expected_data = {
        0x21, 0x66, 0xaf, 0xa3, 0xe2, 0xf9, 0x1d, 0xc3,
        0xc1, 0xd8, 0xf6, 0xa6, 0xcf, 0xbb, 0xbb, 0x23, // ctx
        0xc5, 0x1e, 0xf0, 0xd7, 0x40, 0x1c, 0xa5, 0xea,
        0x6f, 0x85, 0x11, 0x1e, 0x55, 0x74, 0x64, 0xcc // ptx
    };

    auto vc=view.copy_to_vector();
    ASSERT_EQ(expected_data, vc);
//    ASSERT_EQ(expected_data, view.copy_to_vector());
}

TEST(rho_streams, aes_vector) {
    const json json_config = R"({
         "type": "pipe_in_stream",
         "id": "rho_ptx",
         "source": {
                 "type": "block",
                 "init_frequency": "only_once",
                 "algorithm": "AES",
                 "round": 10,
                 "block_size": 16,
                 "plaintext": {
                     "type": "pipe_out_stream",
                     "id": "rho_ptx"
                 },
                 "key_size": 16,
                 "key": {
                     "type": "false_stream"
                 },
                 "iv": {
                     "type": "pcg32_stream"
                 }
             }

     }
    )"_json;

    json aes_json_config = {{"type", "block"},
                            {"init_frequency", "only_once"},
                            {"algorithm", "AES"},
                            {"round", 10},
                            {"block_size", 16},
                            {"plaintext", {{"type", "test_stream"}}},
                            {"key_size", 16},
                            {"key", {{"type", "false_stream"}}},
                            {"iv", {{"type", "pcg32_stream"}}}};

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> map;

    std::unique_ptr<stream> tested_stream = make_stream(json_config, seeder, map, 16);
    std::vector<value_type> prev(16);

    for (unsigned i = 0; i < testing_size; ++i) {
        aes_json_config["plaintext"]["outputs"] = {prev};
        std::unique_ptr<stream> reference_stream = make_stream(aes_json_config, seeder, map, 16);

        vec_cview tested_view = tested_stream->next();
        vec_cview reference_view = reference_stream->next();

        ASSERT_EQ(reference_view.copy_to_vector(), tested_view.copy_to_vector());
        std::copy(tested_view.begin(), tested_view.end(), prev.begin());
    }
}

TEST(pipe_streams, pipe_in_fist) {
    const json json_in = R"({
         "type": "pipe_in_stream",
         "id": "id",
         "source": {
             "type": "counter"
         }

     }
    )"_json;
    const json json_out = R"({
         "type": "pipe_out_stream",
         "id": "id"
     }
    )"_json;

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> map;

    std::unique_ptr<stream> pipe_in = make_stream(json_in, seeder, map, 16);
    std::unique_ptr<stream> pipe_out = make_stream(json_out, seeder, map, 16);

    for (unsigned i = 0; i < 4; ++i) {
        vec_cview in_view = pipe_in->next();
        vec_cview out_view = pipe_out->next();

        ASSERT_EQ(in_view.copy_to_vector(), out_view.copy_to_vector());
    }
}

TEST(pipe_streams, pipe_out_fist) {
    const json json_in = R"({
         "type": "pipe_in_stream",
         "id": "id",
         "source": {
             "type": "counter"
         }

     }
    )"_json;
    const json json_out = R"({
         "type": "pipe_out_stream",
         "id": "id"
     }
    )"_json;

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> map;

    std::unique_ptr<stream> pipe_out = make_stream(json_out, seeder, map, 16);
    std::unique_ptr<stream> pipe_in = make_stream(json_in, seeder, map, 16);

    for (unsigned i = 0; i < 4; ++i) {
        vec_cview in_view = pipe_in->next();
        vec_cview out_view = pipe_out->next();

        ASSERT_EQ(in_view.copy_to_vector(), out_view.copy_to_vector());
    }
}

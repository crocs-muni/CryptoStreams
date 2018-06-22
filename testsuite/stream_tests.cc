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

TEST(column_streams, basic_test_with_counter) {
    json json_config = {{"size", 5}, {"source", {{"type", "counter"}}}};

    seed_seq_from<pcg32> seeder(testsuite::seed1);
    std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> map;

    std::unique_ptr<column_stream> stream =
        std::make_unique<column_stream>(json_config, seeder, map, 5);

    // TODO: finish column test
    //    for(auto it : stream->next()) {
    //        std::cout << it << std::endl;
    //    }
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

    // fixed expected data AES 10 rounds, with PTX choosen by PCG32 with fixed seed testsuite::seed1
#if defined(__clang__) // if compiler is clang, data are different from gcc :(
    std::vector<value_type> expected_data = {
        0x99, 0x72, 0x5d, 0x0a, 0xac, 0xcb, 0x1a, 0x99,
        0x50, 0xe6, 0x27, 0x53, 0x00, 0x87, 0x8b, 0x57, // ctx
        0x02, 0x6c, 0xc7, 0x6f, 0xaf, 0x91, 0x91, 0xb4,
        0x82, 0x4c, 0x67, 0x8c, 0x22, 0x66, 0x39, 0x90 // ptx
    };
#elif defined(__GNUC__) || defined(__GNUG__)
    std::vector<value_type> expected_data = {
        0x21, 0x66, 0xaf, 0xa3, 0xe2, 0xf9, 0x1d, 0xc3,
        0xc1, 0xd8, 0xf6, 0xa6, 0xcf, 0xbb, 0xbb, 0x23, // ctx
        0xc5, 0x1e, 0xf0, 0xd7, 0x40, 0x1c, 0xa5, 0xea,
        0x6f, 0x85, 0x11, 0x1e, 0x55, 0x74, 0x64, 0xcc // ptx
    };
#elif defined(_MSC_VER) // MS data unknown
    std::vector<value_type> expected_data = {
        // TODO
    };
#endif

    ASSERT_EQ(expected_data, view.copy_to_vector());
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
        aes_json_config["plaintext"]["outputs"] = prev;
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

// TODO: add test for creation of the pipes
//       we should test both orders of creation

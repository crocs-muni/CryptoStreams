//
// Created by mhajas on 1/8/18.
//

#include <gtest/gtest.h>
#include <memory>
#include <streams/prngs/testu01-prngs/functions/ulcg/ulcg_generator.h>
#include <streams/prngs/testu01-prngs/testu01_interface.h>
#include <streams/prngs/testu01-prngs/functions/umrg/umrg_generator.h>
#include <streams/prngs/testu01-prngs/functions/uxorshift/uxorshift_generator.h>
#include <eacirc-core/seed.h>


TEST(LCG, basic_test) {
    auto test = std::make_unique<prng::ulcg_generator>(std::make_unique<false_stream>(1), false, 500 + 1, 1, 1);
    // +1 in order to avoid last zero
    std::vector<uint8_t> data(500 * 7);
    test->generate_bits(data.data(), data.size());

    uint32_t j = 0;
    uint32_t iteration = 0;
    uint64_t value = 0;

    for (auto i : data) {
        value += uint64_t(i) << ((iteration % 7) * 8);
        if (++iteration % 7 == 0){
            ASSERT_EQ(++j, value);
            value = 0;
        }
    }
}

TEST(LCG, test_reseed) {
    auto test = std::make_unique<prng::ulcg_generator>(std::make_unique<false_stream>(2), true, 100000, 1, 1);
    std::vector<uint8_t> data(4 * 7);

    for (auto k = 0; k < 1000; k++) {
        test->generate_bits(data.data(), data.size());

        uint32_t j = 0;
        uint32_t iteration = 0;
        uint64_t value = 0;
        for (auto i : data) {
            value += uint64_t(i) << ((iteration % 7) * 8);
            if (++iteration % 7 == 0){
                ASSERT_EQ(++j, value);
                value = 0;
            }
        }
    }
}

TEST(LCG, random_seed_test) {
    seed seed1 = seed::create("1fe40505e131963c");
    seed_seq_from<pcg32> seeder(seed1);

    auto test = std::make_unique<prng::ulcg_generator>(std::make_unique<pcg32_stream >(seeder, 7), true, 9223372036854775783, 4645906587823291368, 0);

    std::vector<uint64_t> data(2); // 8 * int (seed) + one output

    for (auto k = 1; k <= 300000; k++) {
        test->generate_bits(reinterpret_cast<uint8_t *>(data.data()), data.size()*8);
    }
}



TEST(MRG, basic_test) {
    std::vector<long> a = {2975962250, 2909704450};
    uint64_t m = 9223372036854775783;
    size_t viable_bytes = prng::umrg_generator::get_viable_number_of_bytes(m);
    auto test = std::make_unique<prng::umrg_generator>(std::make_unique<counter>(2 * viable_bytes), true, m, a);
    std::vector<uint8_t> data(7);

    uint32_t i = 0;
    for (auto k = 1; k <= 1000; k++) {
        test->generate_bits(data.data(), data.size());

        int j = 0;

        uint64_t output = 0;

        for (auto l = 0; l < 7; l++) {
            output |= uint64_t(data[j++]) << (8 * l);
        }

        ASSERT_EQ(2975962250 * ++i, output);
    }

}

TEST(MRG, random_seed_test) {
    seed seed1 = seed::create("1fe40505e131963c");
    seed_seq_from<pcg32> seeder(seed1);

    std::vector<long> a = {2975962250, 2909704450};
    uint64_t m = 9223372036854775783;
    size_t viable_bytes = prng::umrg_generator::get_viable_number_of_bytes(m);
    auto test = std::make_unique<prng::umrg_generator>(std::make_unique<pcg32_stream>(seeder, 2 * viable_bytes), true, m, a);

    std::vector<uint64_t> data(3); // 8 * int (seed) + one 8 byte output

    for (auto k = 1; k <= 300000; k++) {
        test->generate_bits(reinterpret_cast<uint8_t *>(data.data()), data.size()*8);
    }
}

TEST(XORSHIFT, random_seed_test) {
    seed seed1 = seed::create("1fe40505e131963c");
    seed_seq_from<pcg32> seeder(seed1);

    auto test = std::make_unique<prng::uxorshift_generator>(std::make_unique<pcg32_stream >(seeder, 8 * 4), true);

    std::vector<uint32_t> data(8 + 1); // 8 * int (seed) + one output

    for (auto k = 1; k <= 300000; k++) {
        test->generate_bits(reinterpret_cast<uint8_t *>(data.data()), data.size()*4);
    }
}
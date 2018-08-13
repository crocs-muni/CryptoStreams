//
// Created by mhajas on 1/8/18.
//

#include <gtest/gtest.h>
#include <memory>
#include <eacirc-core/seed.h>
#include <streams/prngs/std-prngs/functions/lcg_generator.h>


TEST(STD_LCG, basic_test) {
    auto test = std::make_unique<prng::lcg_generator>(std::make_unique<counter>(4), true);

    std::vector<uint32_t> data(2);

    for (auto k = 1; k <= 100000; k++) {
        test->generate_bits(reinterpret_cast<uint8_t *>(data.data()), data.size() * 4);

        ASSERT_EQ((k * 48271UL) % 2147483647UL, data[0]);
        ASSERT_EQ((k * 48271UL * 48271UL) % 2147483647UL, data[1]);
    }
}
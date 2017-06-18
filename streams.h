#pragma once

#include "stream.h"
#include <fstream>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include <random>

#ifdef BUILD_estream
#include <streams/estream/estream_stream.h>
#endif

#ifdef BUILD_sha3
#include <streams/sha3/sha3_stream.h>
#endif

#ifdef BUILD_block
#include <streams/block/block_stream.h>
#endif

namespace _impl {

    template <std::uint8_t value>
    struct const_stream : stream {
        const_stream(const std::size_t osize);

        vec_view next() override;

    private:
        std::vector<value_type> _data;
    };

    template <typename Generator>
    struct rng_stream : stream {
        template <typename Seeder>
        rng_stream(Seeder&& seeder, const std::size_t osize);

        vec_view next() override;

    private:
        Generator _rng;
        std::vector<value_type> _data;
    };

} // namespace _impl

/**
 * @brief Stream of data read from a file
 */
struct file_stream : stream {
    file_stream(const json& config, const std::size_t osize);

    vec_view next() override;

private:
    const std::string _path;
    std::basic_ifstream<std::uint8_t> _istream;
    std::vector<value_type> _data;
};

/**
 * @brief Stream of counter
 */
struct counter : stream {
    counter(const std::size_t osize);

    vec_view next() override;

private:
    std::vector<value_type> _data;
};

/**
 * @brief Stream for testing strict avalanche criterion
 *
 * The vector consists 2 parts of same length. The first part is random,
 * the second is copy of the first with one flipped bit
 */
struct sac_stream : stream {
    template <typename Seeder>
    sac_stream(Seeder&& seeder, const std::size_t osize);

    vec_view next() override;

private:
    pcg32 _rng;
    std::vector<value_type> _data;
};

struct sac_fixed_pos_stream : stream {
    template <typename Seeder>
    sac_fixed_pos_stream(Seeder&& seeder, const std::size_t osize, const std::size_t flip_bit_position);

    vec_view next() override;

private:
    pcg32 _rng;
    std::vector<value_type> _data;
    const std::size_t _flip_bit_position;
};

struct sac_2d_all_pos : stream {
    template <typename Seeder>
    sac_2d_all_pos(Seeder&& seeder, const std::size_t osize);

    vec_view next() override;

private:
    pcg32 _rng;
    std::vector<value_type> _data;
    // storing copy is not optimal, can be done faster with more conditions
    std::vector<value_type> _origin_data;
    std::size_t _flip_bit_position;
};

struct column_stream : stream {
    column_stream(const json& config, default_seed_source& seeder, const std::size_t osize);

    vec_view next() override;

private:
    std::size_t _internal_bit_size;
    std::vector<std::vector<value_type>> _buf; // change to array (maxe osize() constexpression), or init it to given size
    std::size_t _position;
    std::unique_ptr<stream> _source;
};

struct column_fixed_position_stream : stream {
    column_fixed_position_stream(const json& config, default_seed_source& seeder, const std::size_t osize, const std::size_t position);

    vec_view next() override;

private:
    std::vector<value_type> _data; // change to array (maxe osize() constexpression), or init it to given size
    const std::size_t _position;
    std::unique_ptr<stream> _source;
};


/**
 * \brief Stream of true bits
 */
using true_stream = _impl::const_stream<std::numeric_limits<std::uint8_t>::max()>;

/**
 * \brief Stream of false bits
 */
using false_stream = _impl::const_stream<std::numeric_limits<std::uint8_t>::min()>;

/**
 * \brief Stream of data produced by Merseine Twister
 */
using mt19937_stream = _impl::rng_stream<std::mt19937>;

/**
 * \brief Stream of data produced by PCG (Permutation Congruential Generator)
 */
using pcg32_stream = _impl::rng_stream<pcg32>;

std::unique_ptr<stream> make_stream(const json& config, default_seed_source& seeder, std::size_t osize);
void stream_to_dataset(dataset &set, std::unique_ptr<stream>& source);

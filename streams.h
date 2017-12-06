#pragma once

#include "stream.h"
#include <fstream>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include <random>

#ifdef BUILD_testsuite
#include <testsuite/test-utils/test_streams.h>
#endif

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
        const_stream(const std::size_t osize)
                : stream(osize)
                , _data(osize) {
            std::fill_n(_data.begin(), osize, value);
        }

        vec_cview next() override {
            return make_cview(_data);
        }

    private:
        std::vector<value_type> _data;
    };

    template <typename Generator>
    struct rng_stream : stream {
        template <typename Seeder>
        rng_stream(Seeder&& seeder, const std::size_t osize)
                : stream(osize)
                , _rng(std::forward<Seeder>(seeder))
                , _data(osize) {}

        vec_cview next() override {
            std::generate_n(_data.data(), osize(), [this]() {
                return std::uniform_int_distribution<std::uint8_t>()(_rng);
            });
            return make_cview(_data);
        }

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

    vec_cview next() override;

private:
    const std::string _path;
    std::ifstream _istream;
    std::vector<value_type> _data;
};

/**
 * @brief Stream of counter
 */
struct counter : stream {
    counter(const std::size_t osize);

    vec_cview next() override;

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
    sac_stream(Seeder&& seeder, const std::size_t osize)
            : stream(osize)
            , _rng(std::forward<Seeder>(seeder))
            , _data(osize) {
        if (osize % 2 == 1)
            throw std::runtime_error(
                    "Stream's osize has to be even (so it contains 2 vectors of same legth).");
    }

    vec_cview next() override {
        std::generate_n(_data.data(), osize() / 2, [this]() {
            return std::uniform_int_distribution<std::uint8_t>()(_rng);
        });

        std::copy_n(_data.begin(), osize() / 2, _data.begin() + osize() / 2);

        std::uniform_int_distribution<std::size_t> dist{0, osize() / 2 * 8};
        std::size_t pos = dist(_rng) + osize() / 2 * 8;

        _data[pos / 8] ^= (1 << (pos % 8)); // TODO: valgrind invalid read and write, [pos / 8] is out of range
        return make_cview(_data);
    }

private:
    pcg32 _rng;
    std::vector<value_type> _data;
};

struct sac_fixed_pos_stream : stream {
    template <typename Seeder>
    sac_fixed_pos_stream(Seeder&& seeder, const std::size_t osize, const std::size_t flip_bit_position)
            : stream(osize)
            , _rng(std::forward<Seeder>(seeder))
            , _data(osize)
            , _flip_bit_position(flip_bit_position) {
        if (osize % 2 == 1)
            throw std::runtime_error(
                    "Stream's osize has to be even (so it contains 2 vectors of same legth).");
        if (_flip_bit_position >= osize*8)
            throw std::runtime_error(
                    "Position of the flipped bit has to be in range of vector size.");

    }

    vec_cview next() override {
        std::generate_n(_data.data(), osize() / 2, [this]() {
            return std::uniform_int_distribution<std::uint8_t>()(_rng);
        });

        std::copy_n(_data.begin(), osize() / 2, _data.begin() + osize() / 2);

        _data[_flip_bit_position / 8] ^= (1 << (_flip_bit_position % 8));
        return make_cview(_data);
    }

private:
    pcg32 _rng;
    std::vector<value_type> _data;
    const std::size_t _flip_bit_position;
};

struct sac_2d_all_pos : stream {
    template <typename Seeder>
    sac_2d_all_pos(Seeder&& seeder, const std::size_t osize)
            : stream(osize)
            , _rng(std::forward<Seeder>(seeder))
            , _data(osize)
            , _origin_data(osize)
            , _flip_bit_position(0)
    {}

    vec_cview next() override {
        if (_flip_bit_position == 0) {
            std::generate_n(_data.data(), osize(), [this]() {
                return std::uniform_int_distribution<std::uint8_t>()(_rng);
            });
            std::copy_n(_data.begin(), osize(), _origin_data.begin());
        } else {
            std::copy_n(_origin_data.begin(), osize(), _data.begin());

            _data[_flip_bit_position / 8] ^= (1 << (_flip_bit_position % 8));
        }

        _flip_bit_position = (_flip_bit_position + 1) % (osize() * 8);

        return make_cview(_data);
    }

private:
    pcg32 _rng;
    std::vector<value_type> _data;
    // storing copy is not optimal, can be done faster with more conditions
    std::vector<value_type> _origin_data;
    std::size_t _flip_bit_position;
};

struct column_stream : stream {
    column_stream(const json& config, default_seed_source& seeder, const std::size_t osize);

    vec_cview next() override;

private:
    std::size_t _internal_bit_size;
    std::vector<std::vector<value_type>> _buf; // change to array (maxe osize() constexpression), or init it to given size
    std::size_t _position;
    std::unique_ptr<stream> _source;
};

struct column_fixed_position_stream : stream {
    column_fixed_position_stream(const json& config, default_seed_source& seeder, const std::size_t osize, const std::size_t position);

    vec_cview next() override;

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

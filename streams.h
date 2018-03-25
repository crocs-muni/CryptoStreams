#pragma once

#include "stream.h"
#include <fstream>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include <eacirc-core/optional.h>
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
                : stream(osize) {
            std::fill_n(_data.begin(), osize, value);
        }

        vec_cview next() override {
            return make_cview(_data);
        }
    };

    template <typename Generator>
    struct rng_stream : stream {
        template <typename Seeder>
        rng_stream(Seeder&& seeder, const std::size_t osize)
                : stream(osize)
                , _rng(std::forward<Seeder>(seeder)) {}

        vec_cview next() override {
            std::generate_n(_data.data(), osize(), [this]() {
                return std::uniform_int_distribution<std::uint8_t>()(_rng);
            });
            return make_cview(_data);
        }

    private:
        Generator _rng;
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
};

/**
 * @brief Stream outputing a constant value
 */
struct single_value_stream : stream {
    single_value_stream(const json& config, default_seed_source &seeder, const std::size_t osize);

    vec_cview next() override;
};

/**
 * @brief Stream of counter
 */
struct counter : stream {
    counter(const std::size_t osize);

    vec_cview next() override;
};

/**
 * @brief Stream of counter starting from random number
 */
struct random_start_counter : counter {
    random_start_counter(default_seed_source &seeder, const std::size_t osize);
};

/**
 * @brief Stream XORing two parts of internal stream
 */
struct xor_stream : stream {
    template <typename Seeder>
    xor_stream(const json& config, Seeder&& seeder, const std::size_t osize);

    vec_cview next() override;
private:
    std::unique_ptr<stream> _source;
};

/**
 * @brief Stream XORing two parts of internal stream
 */
struct rnd_plt_ctx_stream : stream {
    template <typename Seeder>
    rnd_plt_ctx_stream(const json& config, Seeder&& seeder, const std::size_t osize);

    vec_cview next() override;
private:
    std::unique_ptr<stream> _rng;
    std::unique_ptr<stream> _source;
};

/**
 * @brief Stream for testing strict avalanche criterion
 *
 * Stateful generator
 * Two consecussive vectors have following property:
 * The first vector is random and the second is a copy
 * of the first with one flipped bit.
 */
struct sac_stream : stream {
    template <typename Seeder>
    sac_stream(Seeder&& seeder, const std::size_t osize)
            : stream(osize)
            , _rng(std::forward<Seeder>(seeder))
            , _first(true) { }

    vec_cview next() override {
        if (_first) {
            std::generate_n(_data.data(), osize(), [this]() {
                return std::uniform_int_distribution<std::uint8_t>()(_rng);
            });
        } else {
            std::uniform_int_distribution<std::size_t> dist{0, (osize() * 8) - 1};
            std::size_t pos = dist(_rng);

            _data[pos / 8] ^= (1 << (pos % 8));
        }
        _first ^= true;
        return make_cview(_data);
    }

private:
    pcg32 _rng;
    bool _first;
};

struct sac_fixed_pos_stream : stream {
    template <typename Seeder>
    sac_fixed_pos_stream(Seeder&& seeder, const std::size_t osize, const std::size_t flip_bit_position)
            : stream(osize)
            , _rng(std::forward<Seeder>(seeder))
            , _flip_bit_position(flip_bit_position)
            , _first(true) {
        if (_flip_bit_position >= osize*8)
            throw std::runtime_error(
                    "Position of the flipped bit has to be in range of vector size.");

    }

    vec_cview next() override {
        if (_first) {
            std::generate_n(_data.data(), osize(), [this]() {
                return std::uniform_int_distribution<std::uint8_t>()(_rng);
            });
        } else {
            _data[_flip_bit_position / 8] ^= (1 << (_flip_bit_position % 8));
        }
        _first ^= true;
        return make_cview(_data);
    }

private:
    pcg32 _rng;
    const std::size_t _flip_bit_position;
    bool _first;
};

struct sac_2d_all_pos : stream {
    template <typename Seeder>
    sac_2d_all_pos(Seeder&& seeder, const std::size_t osize)
            : stream(osize)
            , _rng(std::forward<Seeder>(seeder))
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
    // storing copy is not optimal, can be done faster with more conditions
    std::vector<value_type> _origin_data;
    std::size_t _flip_bit_position;
};

struct hw_counter : stream {
    template<typename Seeder>
    hw_counter(const json& config, Seeder &&seeder, const std::size_t osize)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _origin_data(osize)
        , _increase_hw(config.value("increase_hw", true))
        , _randomize_overflow(config.value("randomize_overflow", false))
        , _cur_hw(static_cast<uint64_t>(config.value("hw", 1)))
    {
        bool randomize_start = config.value("randomize_start", false);

        if (_cur_hw == 0 || _cur_hw > osize * 8){
            throw std::runtime_error("Invalid Hamming weight for the given output size");
        }
        if (_randomize_overflow && _increase_hw){
            throw std::runtime_error("Randomize overflow and increase counter are mutually exclusive");
        }

        if (randomize_start) {
            randomize();
        } else {
            std::fill_n(_origin_data.begin(), osize, 0);
        }

        combination_init();
    }

    vec_cview next() override;


private:
    void randomize(){
        std::generate_n(_origin_data.data(), osize(), [this]() {
            return std::uniform_int_distribution<std::uint8_t>()(_rng);
        });
    }

    void combination_init() {
        _cur_positions.clear();
        for (std::size_t i = 0; i < _cur_hw; ++i) {
            _cur_positions.push_back(i);
        }
    }

    bool combination_next() {
        const auto size = static_cast<int64_t>(_cur_positions.size());
        auto idx = size - 1;

        if (_cur_positions[idx] == osize()*8 - 1) {
            do {
                idx -= 1;
            } while (idx >= 0 && _cur_positions[idx] + 1 == _cur_positions[idx + 1]);

            if (idx < 0) {
                return false;
            }

            for (auto j = idx + 1; j < size; ++j) {
                _cur_positions[j] = _cur_positions[idx] + j - idx + 1;
            }
        }
        _cur_positions[idx]++;
        return true;
    }

    pcg32 _rng;
    std::vector<value_type> _origin_data;
    const bool _increase_hw;
    const bool _randomize_overflow;
    std::size_t _cur_hw;
    std::vector<std::size_t> _cur_positions;
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

std::unique_ptr<stream> make_stream(const json& config, default_seed_source& seeder, std::size_t osize,
                                    core::optional<stream *> stream = core::nullopt_t{});
void stream_to_dataset(dataset &set, std::unique_ptr<stream>& source);

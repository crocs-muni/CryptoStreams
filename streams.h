#pragma once

#include "stream.h"
#include <eacirc-core/json.h>
#include <eacirc-core/optional.h>
#include <eacirc-core/random.h>
#include <fstream>
#include <random>

#ifdef BUILD_testsuite
#include <testsuite/test_utils/test_streams.h>
#endif

#ifdef BUILD_stream_ciphers
#include <streams/stream_ciphers/stream_stream.h>
#endif

#ifdef BUILD_block
#include <streams/block/block_stream.h>
#endif

#ifdef BUILD_prngs
#include <streams/prngs/prng_stream.h>
#endif

#ifdef BUILD_hash
#include <streams/hash/hash_stream.h>
#endif

namespace _impl {

template <std::uint8_t value> struct const_stream : stream {
    const_stream(const std::size_t osize)
        : stream(osize) {
        std::fill_n(_data.begin(), osize, value);
    }

    vec_cview next() override { return make_cview(_data); }
};

template <typename Generator> struct rng_stream : stream {
    template <typename Seeder>
    rng_stream(Seeder &&seeder, const std::size_t osize)
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
 * @brief Stream of data updated from outside (using set_data)
 */
struct dummy_stream : stream {
    dummy_stream(const std::size_t osize)
        : stream(osize) {}

    vec_cview next() override { return make_cview(_data); }
};

/**
 * @brief Stream of data read from a file
 */
struct file_stream : stream {
    file_stream(const json &config, const std::size_t osize);

    vec_cview next() override;

private:
    const std::string _path;
    std::ifstream _istream;
};

/**
 * @brief Stream outputing a constant vector
 */
struct const_stream : stream {
    const_stream(const json &config, const std::size_t osize);

    vec_cview next() override;

private:
    static void fromHex(std::vector<value_type> &res, const std::string &hex);
    std::vector<value_type> _data;
};

/**
 * @brief Stream outputing a constant value for n iterations
 */
struct repeating_stream : stream {
    repeating_stream(
        const json &config,
        default_seed_source &seeder,
        std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
        const std::size_t osize);

    vec_cview next() override;

private:
    std::unique_ptr<stream> _source;
    const unsigned _period;
    unsigned _i;
};

/**
 * @brief Stream outputing a constant value
 */
struct single_value_stream : stream {
    single_value_stream(
        const json &config,
        default_seed_source &seeder,
        std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
        const std::size_t osize);

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
    xor_stream(const json &config,
               Seeder &&seeder,
               std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
               const std::size_t osize);

    vec_cview next() override;

private:
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
    sac_stream(Seeder &&seeder, const std::size_t osize)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _first(true) {}

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
    sac_fixed_pos_stream(Seeder &&seeder,
                         const std::size_t osize,
                         const std::size_t flip_bit_position)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _flip_bit_position(flip_bit_position)
        , _first(true) {
        if (_flip_bit_position >= osize * 8)
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
    sac_2d_all_pos(Seeder &&seeder, const std::size_t osize)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _origin_data(osize)
        , _flip_bit_position(0) {}

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
    template <typename Seeder>
    hw_counter(const json &config, Seeder &&seeder, const std::size_t osize)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _origin_data(osize)
        , _increase_hw(config.value("increase_hw", true))
        , _randomize_overflow(config.value("randomize_overflow", false))
        , _cur_hw(static_cast<uint64_t>(config.value("hw", 1))) {
        bool randomize_start = config.value("randomize_start", false);

        if (_cur_hw == 0 || _cur_hw > osize * 8) {
            throw std::runtime_error("Invalid Hamming weight for the given output size");
        }
        if (_randomize_overflow && _increase_hw) {
            throw std::runtime_error(
                "Randomize overflow and increase counter are mutually exclusive");
        }

        if (randomize_start) {
            randomize();
        } else {
            std::fill_n(_origin_data.begin(), osize, 0);
        }

        auto initial_state = config.find("initial_state");
        if (initial_state != config.end()) {
            combination_init_state(*initial_state, osize);
        } else {
            combination_init();
        }
    }

    hw_counter(const std::size_t osize)
        : stream(osize)
        , _rng()
        , _origin_data(osize)
        , _increase_hw(true)
        , _randomize_overflow(false)
        , _cur_hw(1) {
        if (_cur_hw == 0 || _cur_hw > osize * 8) {
            throw std::runtime_error("Invalid Hamming weight for the given output size");
        }
        if (_randomize_overflow && _increase_hw) {
            throw std::runtime_error(
                "Randomize overflow and increase counter are mutually exclusive");
        }

        std::fill_n(_origin_data.begin(), osize, 0);

        combination_init();
    }

    vec_cview next() override;

private:
    void randomize() {
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

    void combination_init_state(const json &initial_state, size_t osize) {
        if (!initial_state.is_array()) {
            throw std::runtime_error("initial_state has to be an array of integers");
        }
        if (initial_state.size() != _cur_hw) {
            throw std::runtime_error("initial_state length is inconsistent with hw parameter");
        }

        _cur_positions.clear();
        _cur_positions.reserve(_cur_hw);
        std::size_t prevValue = 0;
        std::size_t iter = 0;
        for (auto const &cur : initial_state) {
            if (!cur.is_number()) {
                throw std::runtime_error("All elements need to be a number");
            }
            auto curValue = cur.get<size_t>();
            if (iter > 0 && curValue <= prevValue) {
                throw std::runtime_error("Element indices have to be strictly increasing sequence");
            }
            if (curValue >= osize * 8) {
                throw std::runtime_error("Element index is out of bounds");
            }

            _cur_positions.push_back(curValue);
            prevValue = curValue;
            iter += 1;
        }
    }

    bool combination_next() {
        const auto size = static_cast<int64_t>(_cur_positions.size());
        auto idx = size - 1;

        if (_cur_positions[idx] == osize() * 8 - 1) {
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
    column_stream(const json &config,
                  default_seed_source &seeder,
                  std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
                  const std::size_t osize);

    vec_cview next() override;

private:
    std::size_t _internal_bit_size;
    std::vector<std::vector<value_type>>
        _buf; // change to array (maxe osize() constexpression), or init it to given size
    std::size_t _position;
    std::unique_ptr<stream> _source;
};

struct column_fixed_position_stream : stream {
    column_fixed_position_stream(
        const json &config,
        default_seed_source &seeder,
        std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
        const std::size_t osize,
        const std::size_t position);

    vec_cview next() override;

private:
    const std::size_t _position;
    std::unique_ptr<stream> _source;
};

/**
 * @brief Stream with bits generated according to bernoulli distribution.
 */
struct bernoulli_distribution_stream : stream {
    template <typename Seeder>
    bernoulli_distribution_stream(const json &config, Seeder &&seeder, const std::size_t osize)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _distribution(std::bernoulli_distribution(double(config.value("p", 0.5)))) {}

    vec_cview next() override {
        std::generate_n(_data.data(), osize(), [this]() {
            uint8_t out = 0;
            for (unsigned i = 0; i < 8; ++i) {
                out |= (_distribution(_rng) << i);
            }
            return out;
        });
        return make_cview(_data);
    }

private:
    pcg32 _rng;
    std::bernoulli_distribution _distribution;
};

/**
 * @brief Stream with bits generated according to binomial distribution.
 */
struct binomial_distribution_stream : stream {
    template <typename Seeder>
    binomial_distribution_stream(const json &config, Seeder &&seeder, const std::size_t osize)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _distribution(uint8_t(config.value("max_value", std::numeric_limits<uint8_t>::max())),
                        double(config.value("p", 0.5))) {}

    vec_cview next() override {
        std::generate_n(_data.data(), osize(), [this]() { return _distribution(_rng); });
        return make_cview(_data);
    }

private:
    pcg32 _rng;
    std::binomial_distribution<uint8_t> _distribution;
};

/**
 * @brief Stream with bits generated according to normal distribution.
 *
 * Cutted distribution tails outside of 4 times standard deviation.
 */
struct normal_distribution_stream : stream {
    template <typename Seeder>
    normal_distribution_stream(const json &config, Seeder &&seeder, const std::size_t osize)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _distribution(double(config.value("mean", 0)), double(config.value("std_dev", 1.0))) {}

    vec_cview next() override {
        std::generate_n(_data.data(), osize(), [this]() {
            double res;
            double sigma_count = 4.0;
            double std_dev = _distribution.stddev();
            do {
                res = _distribution(_rng);
            } while (res < -sigma_count * std_dev or res > sigma_count * std_dev);
            double normalized_value =
                res / (2 * sigma_count * std_dev) + 0.5; // normalized to [0, 1];
            return uint8_t(normalized_value * std::numeric_limits<uint8_t>::max());
        });
        return make_cview(_data);
    }

private:
    pcg32 _rng;
    std::normal_distribution<double> _distribution;
};

/**
 * @brief Stream with bits generated according to poisson distribution.
 */
struct poisson_distribution_stream : stream {
    template <typename Seeder>
    poisson_distribution_stream(const json &config, Seeder &&seeder, const std::size_t osize)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _distribution(double(config.value("mean", std::numeric_limits<uint8_t>::max() / 2))) {}

    vec_cview next() override {
        std::generate_n(_data.data(), osize(), [this]() { return _distribution(_rng); });
        return make_cview(_data);
    }

private:
    pcg32 _rng;
    std::poisson_distribution<uint8_t> _distribution;
};

/**
 * @brief Stream with bits generated according to exponential distribution.
 */
struct exponential_distribution_stream : stream {
    template <typename Seeder>
    exponential_distribution_stream(const json &config, Seeder &&seeder, const std::size_t osize)
        : stream(osize)
        , _rng(std::forward<Seeder>(seeder))
        , _distribution(double(config.value("lambda", 1))) {}

    vec_cview next() override {
        std::generate_n(_data.data(), osize(), [this]() { return uint8_t(_distribution(_rng)); });
        return make_cview(_data);
    }

private:
    pcg32 _rng;
    std::exponential_distribution<double> _distribution;
};

/**
 * @brief Pipe's sink - stores pointer on the intersnal stream to
 * the pipes hashtable
 */
struct pipe_in_stream : stream {
    pipe_in_stream(const json &config,
                   default_seed_source &seeder,
                   std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
                   const std::size_t osize);

    vec_cview next() override { return (*_source)->next(); }

private:
    std::shared_ptr<std::unique_ptr<stream>> _source;
};

/**
 * @brief Pipe's source
 */
struct pipe_out_stream : stream {
    pipe_out_stream(
        const json &config,
        std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes)
        : stream(0) {
        std::string pipe_id = config.at("id");

        auto search = pipes.find(pipe_id);
        if (search == pipes.end()) {
            // pipe_id is not yet in hashtable, create new empty entry
            _source =
                std::make_shared<std::unique_ptr<stream>>(std::unique_ptr<stream>(nullptr));
            pipes[pipe_id] = _source;
        } else {
            // pipe_id is in hashtable, use ptr from pipe_in
            _source = pipes[pipe_id];
        }
    }

    vec_cview next() override { return (*_source)->get_data(); }

private:
    std::shared_ptr<std::unique_ptr<stream>> _source;
};

/**
 * @brief Ordered tuple of streams
 */
struct tuple_stream : stream {
    tuple_stream(const json &config,
                 default_seed_source &seeder,
                 std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
                 const std::size_t osize);

    vec_cview next() override {
        auto beg = _data.begin();
        for (auto& source : _sources) {
            vec_cview v = source->next();
            copy(v.begin(), v.end(), beg);
            beg += v.end() - v.begin();
        }

        return make_cview(_data);
    }

private:
    std::vector<std::unique_ptr<stream>> _sources;
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

std::unique_ptr<stream>
make_stream(const json &config,
            default_seed_source &seeder,
            std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
            std::size_t osize);
void stream_to_dataset(dataset &set, std::unique_ptr<stream> &source);

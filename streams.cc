#include "streams.h"
#include <algorithm>
#include <fstream>
#include <limits>
#include <random>
#include <string>
#include <eacirc-core/memory.h>
#include <pcg/pcg_random.hpp>

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

    template <std::uint8_t value> struct const_stream : stream {
        const_stream(const std::size_t osize)
            : stream(osize)
            , _data(osize) {
            std::fill_n(_data.begin(), osize, value);
        }

        vec_view next() override { return make_cview(_data); }

    private:
        std::vector<value_type> _data;
    };

    template <typename Generator> struct rng_stream : stream {
        template <typename Seeder>
        rng_stream(Seeder&& seeder, const std::size_t osize)
            : stream(osize)
            , _rng(std::forward<Seeder>(seeder))
            , _data(osize) {}

        vec_view next() override {
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

/**
 * @brief Stream of data read from a file
 */
struct file_stream : stream {
    file_stream(const json& config, const std::size_t osize)
        : stream(osize)
        , _path(config.at("path").get<std::string>())
        , _istream(_path, std::ios::binary)
        , _data(osize) {}

    vec_view next() override {
        _istream.read(_data.data(), osize());

        if (_istream.fail())
            throw std::runtime_error("I/O error while reading a file " + _path);
        if (_istream.eof())
            throw std::runtime_error("end of file " + _path + " reached, not enough data!");

        return make_cview(_data);
    }

private:
    const std::string _path;
    std::basic_ifstream<std::uint8_t> _istream;
    std::vector<value_type> _data;
};

/**
 * @brief Stream of counter
 */
struct counter : stream {
    counter(const std::size_t osize)
        : stream(osize)
        , _data(osize) {
        std::fill(_data.begin(), _data.end(), std::numeric_limits<value_type>::min());
    }

    vec_view next() override {
        for (value_type& value : _data) {
            if (value != std::numeric_limits<value_type>::max()) {
                ++value;
                break;
            }
            value = std::numeric_limits<value_type>::min();
        }
        return make_cview(_data);
    }

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

    vec_view next() override {
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

    vec_view next() override {
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
        , _flip_bit_position(0) { }

    vec_view next() override {
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
    column_stream(const json& config, default_seed_source& seeder, const std::size_t osize)
        : stream(osize)
        , _internal_bit_size(std::size_t(config.at("size")) * 8)
        , _buf(_internal_bit_size)
        , _position(0)
        , _source(make_stream(config.at("stream"), seeder, _internal_bit_size/8)) {
        for (auto &v : _buf)
            v.resize(osize);
    }

    vec_view next() override {
        // regenerate the buffer
        if ((_position % _internal_bit_size) == 0) {
            _position = 0;

            // memset _buf to 0; change to STL?
            for (auto &vec : _buf)
                for (auto &val : vec)
                    val = 0;

            for (std::size_t i = 0; i < osize() * 8; ++i) {
                auto vec = _source->next().data();

                // something like matrix transpose
                for (std::size_t j = 0; j < _internal_bit_size; ++j) {
                    _buf[j][i/8] += ((vec[j/8] & (0x1 << (j % 8))) >> j) << (i % 8);
                }

            }
        }

        return make_cview(_buf[_position++]); // return and increment
    }

private:
    std::size_t _internal_bit_size;
    std::vector<std::vector<value_type>> _buf; // change to array (maxe osize() constexpression), or init it to given size
    std::size_t _position;
    std::unique_ptr<stream> _source;
};

struct column_fixed_position_stream : stream {
    column_fixed_position_stream(const json& config, default_seed_source& seeder, const std::size_t osize, const std::size_t position)
        : stream(osize)
        , _data(osize)
        , _position(position)
        , _source(make_stream(config.at("stream"), seeder, std::size_t(config.at("size")))) { }

    vec_view next() override {
        for (auto &val : _data)
            val = 0;

        std::size_t rev_pos = 7 - (_position % 8);

        for (std::size_t i = 0; i < osize() * 8; ++i) {
            auto vec = _source->next().data();

            std::size_t rev_i = 7 - (i % 8);
            _data[i/8] += ((vec[_position/8] & (0x1 << rev_pos)) >> rev_pos) << rev_i;

        }

        return make_cview(_data); // return and increment
    }

private:
    std::vector<value_type> _data; // change to array (maxe osize() constexpression), or init it to given size
    const std::size_t _position;
    std::unique_ptr<stream> _source;
};

std::unique_ptr<stream>
make_stream(const json& config, default_seed_source& seeder, std::size_t osize = 0) {
    const std::string type = config.at("type");

    if (osize == 0)
        throw std::runtime_error("Stream's osize " + type + " is not set in parent stream.");

    if (type == "file-stream")
        return std::make_unique<file_stream>(config, osize);
    else if (type == "true-stream")
        return std::make_unique<true_stream>(osize);
    else if (type == "false-stream")
        return std::make_unique<false_stream>(osize);
    else if (type == "mt19937-stream")
        return std::make_unique<mt19937_stream>(seeder, osize);
    else if (type == "pcg32-stream")
        return std::make_unique<pcg32_stream>(seeder, osize);
    else if (type == "counter")
        return std::make_unique<counter>(osize);
    else if (type == "sac")
        return std::make_unique<sac_stream>(seeder, osize*2);
    else if (type == "sac-fixed-position") {
        const std::size_t pos = std::size_t(config.at("position"));
        return std::make_unique<sac_fixed_pos_stream>(seeder, osize*2, pos);
    }
    else if (type == "sac-2d-all-positions")
        return std::make_unique<sac_2d_all_pos>(seeder, osize);
    else if (type == "column")
        return std::make_unique<column_stream>(config, seeder, osize);
    else if (type == "column-fixed-position") {
        const std::size_t pos = std::size_t(config.at("position"));
        return std::make_unique<column_fixed_position_stream>(config, seeder, osize, pos);
    }

#ifdef BUILD_estream
    else if (type == "estream")
        return std::make_unique<estream_stream>(config, seeder, osize);
#endif
#ifdef BUILD_sha3
    else if (type == "sha3")
        return std::make_unique<sha3_stream>(config, seeder, osize);
#endif
#ifdef BUILD_block
    else if (type == "block")
        return std::make_unique<block::block_stream>(config, seeder, osize);
#endif
    throw std::runtime_error("requested stream named \"" + type + "\" does not exist");
}

void stream_to_dataset(dataset& set, std::unique_ptr<stream>& source) {

    auto beg = set.rawdata();
    auto end = set.rawdata() + set.rawsize();

    for (; beg != end;) {
        vec_view n = source->next();
        beg = std::copy(n.begin(), n.end(), beg);
    }
}

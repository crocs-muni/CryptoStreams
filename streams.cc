#include "streams.h"

file_stream::file_stream(const json &config, const std::size_t osize)
    : stream(osize)
    , _path(config.at("path").get<std::string>())
    , _istream(_path, std::ios::binary)
    , _data(osize) {}

vec_cview file_stream::next() {
    _istream.read(reinterpret_cast<char*>(_data.data()), osize());

    if (_istream.fail()) {
        perror("stream failbit (or badbit). error state:");
        throw std::runtime_error("I/O error while reading a file " + _path);
    }
    if (_istream.eof())
        throw std::runtime_error("end of file " + _path + " reached, not enough data!");

    return make_cview(_data);
}

counter::counter(const std::size_t osize)
    : stream(osize)
    , _data(osize) {
    std::fill(_data.begin(), _data.end(), std::numeric_limits<value_type>::min());
}

vec_cview counter::next() {
    for (value_type& value : _data) {
        if (value != std::numeric_limits<value_type>::max()) {
            ++value;
            continue;
        }
        value = std::numeric_limits<value_type>::min();
    }
    return make_cview(_data);
}

column_stream::column_stream(const json& config, default_seed_source& seeder, const std::size_t osize)
    : stream(osize)
    , _internal_bit_size(std::size_t(config.at("size")) * 8)
    , _buf(_internal_bit_size)
    , _position(0)
    , _source(make_stream(config.at("stream"), seeder, _internal_bit_size/8)) {
    for (auto &v : _buf)
        v.resize(osize);
}

vec_cview column_stream::next() {
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

column_fixed_position_stream::column_fixed_position_stream(const json& config, default_seed_source& seeder, const std::size_t osize, const std::size_t position)
    : stream(osize)
    , _data(osize)
    , _position(position)
    , _source(make_stream(config.at("stream"), seeder, std::size_t(config.at("size")))) { }

vec_cview column_fixed_position_stream::next() {
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

std::unique_ptr<stream>
make_stream(const json& config, default_seed_source& seeder, const std::size_t osize = 0) {
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
#ifdef BUILD_testsuite
    else if (type == "test-stream")
        return std::make_unique<testsuite::test_stream>(config);
#endif

#ifdef BUILD_estream
    else if (type == "estream")
        return std::make_unique<estream_stream>(config, seeder, osize);
#endif
#ifdef BUILD_sha3
    else if (type == "sha3")
        return std::make_unique<sha3::sha3_stream>(config, seeder, osize);
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
        vec_cview n = source->next();
        beg = std::copy(n.begin(), n.end(), beg);
    }
}

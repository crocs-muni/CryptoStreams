#include "generator.h"
#include <core/logger.h>
#include <core/random.h>
#include <fstream>
#include <pcg/pcg_random.hpp>

#include "../eacirc/streams.h"

#include <sstream>
#include <iomanip>

static std::ifstream open_config_file(std::string path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("can't open config file " + path);
    return file;
}

static std::string out_name(json const& config) {
    std::stringstream ss;
    std::string a = config.at("algorithm");
    ss << a << "_r";
    ss << std::setw(2) << std::setfill('0') << config.at("round");
    ss << "_b" << config.at("block-size");
    ss << ".bin";
    return ss.str();
}

generator::generator(std::string config)
    : generator(open_config_file(config)) {}

generator::generator(json const& config)
    : _config(config)
    , _seed(seed::create(config.at("seed")))
    , _tv_count(config.at("tv-count"))
    , _o_file(out_name(config.at("stream")), std::ios::binary) {

    seed_seq_from<pcg32> main_seeder(_seed);

    _stream_a = make_stream(config.at("stream"), main_seeder, std::size_t(config.at("tv-size")));
}

void generator::generate() {

    for (std::size_t i = 0; i < _tv_count; ++i) {
        vec_view n = _stream_a->next();
        for (auto o : n)
            _o_file << o;
    }
}

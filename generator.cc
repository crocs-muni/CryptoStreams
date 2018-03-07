#include "generator.h"
#include "streams.h"

#include <eacirc-core/logger.h>
#include <eacirc-core/random.h>
#include <pcg/pcg_random.hpp>

#include <fstream>
#include <sstream>
#include <iomanip>

static std::ifstream open_config_file(const std::string path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("can't open config file " + path);
    return file;
}

static std::string out_name(json const& config) {
    auto fname_it = config.find("file-name");
    if (fname_it != config.end()){
        return fname_it;
    }

    std::stringstream ss;
    json config_ref = config.at("stream");
    // this allows finding name hidden in postprocessing streams
    while (config_ref.find("algorithm") == config_ref.end()) {
        config_ref = config_ref.at("source");
    }
    std::string a = config_ref.at("algorithm");
    ss << a << "_r";
    ss << std::setw(2) << std::setfill('0') << std::size_t(config_ref.at("round"));
    ss << "_b" << config_ref.at("block-size");
    ss << ".bin";
    return ss.str();
}

generator::generator(const std::string config)
    : generator(open_config_file(config)) {}

generator::generator(json const& config)
    : _config(config)
    , _seed(seed::create(config.at("seed")))
    , _tv_count(config.at("tv-count"))
    , _o_file_name(out_name(config)) {

    seed_seq_from<pcg32> main_seeder(_seed);

    _stream_a = make_stream(config.at("stream"), main_seeder, std::size_t(config.at("tv-size")));
}

void generator::generate() {

    std::ofstream o_file(_o_file_name, std::ios::binary);

    for (std::size_t i = 0; i < _tv_count; ++i) {
        vec_cview n = _stream_a->next();
        for (auto o : n)
            o_file << o;
    }
}

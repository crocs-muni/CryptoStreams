#pragma once

#include "stream.h"
#include <eacirc-core/json.h>
#include <eacirc-core/seed.h>
#include <fstream>
#include <iostream>
#include <memory>

struct generator {
    generator(const std::string config);

    generator(std::istream &config)
        : generator(json::parse(config)) {}

    generator(std::istream &&config)
        : generator(json::parse(config)) {}

    generator(json &&config)
        : generator(config) {}

    generator(json const &config);

    void generate();

private:
    const json _config;
    const seed _seed;

    const std::uint64_t _tv_count;

    std::unique_ptr<stream> _stream_a;

    std::string _o_file_name;
};

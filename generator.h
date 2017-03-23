#pragma once

#include "../eacirc/seed.h"
#include <core/json.h>
#include <core/stream.h>
#include <memory>
#include <iostream>
#include <fstream>

struct generator {
    generator(std::string cofig);

    generator(std::istream& config)
        : generator(json::parse(config)) {}

    generator(std::istream&& config)
        : generator(json::parse(config)) {}

    generator(json&& config)
        : generator(config) {}

    generator(json const& config);

    void generate();

private:
    const json _config;
    const seed _seed;

    const std::uint64_t _tv_count;

    std::unique_ptr<stream> _stream_a;

    std::ofstream _o_file;
};

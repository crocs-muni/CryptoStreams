#pragma once

#include "stream.h"
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>

std::unique_ptr<stream> make_stream(const json& config, default_seed_source& seeder, std::size_t osize);
void stream_to_dataset(dataset &set, std::unique_ptr<stream>& source);

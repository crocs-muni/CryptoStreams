#pragma once

#include <eacirc-core/json.h>
#include <eacirc-core/memory.h>
#include <eacirc-core/view.h>
#include <eacirc-core/dataset.h>
#include <eacirc-core/logger.h>
#include <vector>

using value_type = std::uint8_t;
using vec_cview = view<std::vector<value_type>::const_iterator>;
using vec_view = view<std::vector<value_type>::iterator>;

struct stream {
    virtual ~stream() = default;

    virtual vec_cview next() = 0;

    std::size_t osize() const { return _osize; }

protected:
    stream(const std::size_t osize)
        : _osize(osize) {}

private:
    const std::size_t _osize;
};

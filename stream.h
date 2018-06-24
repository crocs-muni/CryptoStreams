#pragma once

#include <eacirc-core/dataset.h>
#include <eacirc-core/json.h>
#include <eacirc-core/logger.h>
#include <eacirc-core/view.h>
#include <unordered_map>
#include <vector>

using value_type = std::uint8_t;
using vec_cview = view<std::vector<value_type>::const_iterator>;
using vec_view = view<std::vector<value_type>::iterator>;

struct stream {
    virtual ~stream() = default;

    virtual vec_cview next() = 0;

    vec_cview get_data() const { return make_cview(_data); }

    void set_data(vec_cview data) { std::copy(data.begin(), data.end(), _data.begin()); }

    std::size_t osize() const { return _osize; }

protected:
    stream(const std::size_t osize)
        : _data(osize)
        , _osize(osize) {}

    std::vector<value_type> _data;

private:
    const std::size_t _osize;
};

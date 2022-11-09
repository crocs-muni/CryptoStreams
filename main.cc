#include "generator.h"
#include "version.h"
#include <eacirc-core/cmd.h>
#include <eacirc-core/logger.h>
#include <eacirc-core/version.h>
#include <limits>

void test_environment() {
    if (std::numeric_limits<std::uint8_t>::max() != 255)
        throw std::range_error("Maximum for unsigned char is not 255");
    if (std::numeric_limits<std::uint8_t>::digits != 8)
        throw std::range_error("Unsigned char does not have 8 bits");
}

struct config {
    bool help = false;
    bool version = false;
    std::string config = "generator.json";
};

static cmd<config> options{{"-h", "--help", "display help message", &config::help},
                           {"-v", "--version", "display program version", &config::version},
                           {"-c", "--config", "=<CFG_FILE> specify the config file to load", &config::config}};

int main(const int argc, const char **argv) try {
    auto cfg = options.parse(make_view(argv, argc));

    if (cfg.help) {
        std::cerr << "Usage: eacirc-streams [options]" << std::endl;

        options.print(std::cerr);
    } else if (cfg.version) {
        std::cerr << "Generator version " CRYPTOSTREAMS_VERSION_TAG << ", eacirc-core: " << VERSION_TAG << std::endl;
    } else {
        test_environment();

        generator app(cfg.config);
        app.generate();
    }

    return 0;
} catch (std::exception &e) {
    logger::error(e.what());
    return 1;
}

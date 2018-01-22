
#include "sha1_factory.h"

namespace others{

sha1_factory::sha1_factory(unsigned int rounds) {

}

int sha1_factory::Init(int others_bitsize) {
    sha1_init(others_bitsize);
    return 0;
}

int sha1_factory::Update(const BitSequence *data, DataLength data_bitsize) {
    sha1_update(/*ctx*/,data,data_bitsize, rounds);
    return 0;
}

    int sha1_factory::Final(BitSequence *others) {
        sha1_final(/*ctx*/,others, rounds);
        return others_interface::Final(others);
    }

}

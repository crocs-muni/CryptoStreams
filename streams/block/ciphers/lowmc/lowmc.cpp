#include "lowmc.h"
namespace block {
namespace lowmc {
constexpr const lowmc_paramset_t lowmc::recom_params[];
#ifdef LOWMC_CACHE
LowMCBase * lowmc::last_instance = nullptr;
#endif
}}

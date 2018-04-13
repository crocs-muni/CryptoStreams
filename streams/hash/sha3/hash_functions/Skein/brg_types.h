/* Original file brg_types.h rewriten by Karel Kubicek <karel.kubicek@mail.muni.cz>
 * to solve the licence issues.
 */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <limits.h>
#include <stdint.h>

typedef uint8_t skein_uint_8t;
typedef uint16_t skein_uint_16t;
typedef uint32_t skein_uint_32t;
typedef uint64_t skein_uint_64t;

#define VOID_RETURN void
#define INT_RETURN int

#if defined(__cplusplus)
}
#endif

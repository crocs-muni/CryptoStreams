/* Original file brg_endian.h rewriten by Karel Kubicek <karel.kubicek@mail.muni.cz>
 * to solve the licence issues.
 *
 * https://stackoverflow.com/a/27054190/
 */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#define IS_BIG_ENDIAN      4321
#define IS_LITTLE_ENDIAN   1234

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN ||                                       \
    defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ ||                           \
    defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) ||                       \
    defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
// It's a big-endian target architecture
#define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN

#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN ||                                  \
    defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ ||                        \
    defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) ||                    \
    defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
// It's a little-endian target architecture
#define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN

#else
#error "I don't know what architecture this is!"
#endif


#if defined(__cplusplus)
}
#endif

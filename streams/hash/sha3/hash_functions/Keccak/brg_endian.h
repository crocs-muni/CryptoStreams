/* Original file brg_endian.h rewriten by Karel Kubicek <karel.kubicek@mail.muni.cz>
 * to solve the licence issues.
 *
 * https://stackoverflow.com/a/27054190/
 */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN ||                                       \
    defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ ||                           \
    defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) ||                       \
    defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
// It's a big-endian target architecture
#define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN

#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN ||                                  \
    defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ ||                        \
    defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) ||                    \
    defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) ||	   \
	defined(_WIN32)
// It's a little-endian target architecture
#define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN

#else
#error "I don't know what architecture this is!"
#endif

/* special handler for IA64, which may be either endianness (?)  */
/* here we assume little-endian, but this may need to be changed */
#if defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
#define PLATFORM_MUST_ALIGN (1)
#ifndef PLATFORM_BYTE_ORDER
#define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif
#endif

#ifndef PLATFORM_MUST_ALIGN
#define PLATFORM_MUST_ALIGN (0)
#endif

#if defined(__cplusplus)
}
#endif

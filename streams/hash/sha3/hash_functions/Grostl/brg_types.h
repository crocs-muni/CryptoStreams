/* Original file brg_types.h rewriten by Karel Kubicek <karel.kubicek@mail.muni.cz>
 * to solve the licence issues.
 */
#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <limits.h>
#include <stdint.h>

#if defined( _MSC_VER ) && ( _MSC_VER >= 1300 )
#  include <stddef.h>
#  define ptrint_t intptr_t
#elif defined( __GNUC__ ) && ( __GNUC__ >= 3 )
#  include <stdint.h>
#  define ptrint_t intptr_t
#else
#  define ptrint_t int
#endif

#define GROSTL_BRG_UI8
typedef uint8_t grostl_uint_8t;
#define GROSTL_BRG_UI16
typedef uint16_t grostl_uint_16t;
#define GROSTL_BRG_UI32
#define li_32(h) 0x##h##u
typedef uint32_t grostl_uint_32t;
#define GROSTL_BRG_UI64
#define li_64(h) 0x##h##ul
typedef uint64_t grostl_uint_64t;

#define GROSTL_RETURN_VALUES
#define VOID_RETURN void
#define INT_RETURN int

#define grostl_u8 grostl_uint_8t
#define grostl_u32 grostl_uint_32t
#define grostl_u64 grostl_uint_64t

#if defined(__cplusplus)
}
#endif


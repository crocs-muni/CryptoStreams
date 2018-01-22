
#ifndef INT_TYPES_H
#define INT_TYPES_H

#if defined( _MSC_VER )

typedef __int8				vortex_int8_t;
typedef unsigned __int8		vortex_uint8_t;
typedef __int16				vortex_int16_t;
typedef unsigned __int16	vortex_uint16_t;
typedef __int32				vortex_int32_t;
typedef unsigned __int32	vortex_uint32_t;
typedef __int64				vortex_int64_t;
typedef unsigned __int64	vortex_uint64_t;

#elif defined( __GNUC__ )

// EACIRC: manual edit: next 8 lines added.
#include <stdint.h>
typedef int8_t				vortex_int8_t;
typedef uint8_t             	vortex_uint8_t;
typedef int16_t				vortex_int16_t;
typedef uint16_t              vortex_uint16_t;
typedef int32_t				vortex_int32_t;
typedef uint32_t            	vortex_uint32_t;
typedef int64_t				vortex_int64_t;
typedef uint64_t           	vortex_uint64_t;

#ifndef __KERNEL__
#include <stddef.h>
#include <inttypes.h>
#endif

#else /* No Compiler platform defined */
#error Must define Compiler based Integer types
#endif

typedef int					vortex_boolean_t;


#endif /* INT_TYPES_H */

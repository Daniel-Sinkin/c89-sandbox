/* src/types.h */
#ifndef DANS_TYPES_H
#define DANS_TYPES_H

#include <limits.h>
#include <stddef.h>

#if defined(DANS_TYPES_DEFINED)
#    error "dans/types.h included twice"
#endif
#define DANS_TYPES_DEFINED

#if defined(__GNUC__) || defined(__clang__)
#    define DANS_INLINE __inline__
#elif defined(_MSC_VER)
#    define DANS_INLINE __inline
#else
#    define DANS_INLINE
#endif

/* --- 8-bit --- */
typedef signed char dans_i8;
typedef unsigned char dans_u8;

/* --- 16-bit --- */
#if SHRT_MAX == 32767
typedef short dans_i16;
typedef unsigned short dans_u16;
#else
#    error "Cannot find a 16-bit integer type"
#endif

/* --- 32-bit --- */
#if INT_MAX == 2147483647
typedef int dans_i32;
typedef unsigned int dans_u32;
#elif LONG_MAX == 2147483647L
typedef long dans_i32;
typedef unsigned long dans_u32;
#else
#    error "Cannot find a 32-bit integer type"
#endif

/* --- 64-bit --- */
#if LONG_MAX == 9223372036854775807L
typedef long dans_i64;
typedef unsigned long dans_u64;
#elif defined(LLONG_MAX) && LLONG_MAX == 9223372036854775807LL
typedef long long dans_i64;
typedef unsigned long long dans_u64;
#else
#    error "Cannot find a 64-bit integer type"
#endif

/* --- floating point --- */
typedef float dans_f32;
typedef double dans_f64;

/* --- size type --- */
typedef size_t dans_usize;

#if defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 8
#    define DANS_USIZE_IS_64
#elif defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 4
#    define DANS_USIZE_IS_32
#elif SIZE_MAX == 0xFFFFFFFFFFFFFFFFULL
#    define DANS_USIZE_IS_64
#elif SIZE_MAX == 0xFFFFFFFFU
#    define DANS_USIZE_IS_32
#else
#    error "Unsupported pointer size"
#endif

/* --- bool --- */
typedef int dans_bool;
#define DANS_TRUE 1
#define DANS_FALSE 0

/* --- static assert --- */
#define DANS_STATIC_ASSERT(cond, name) typedef char dans_static_assert_##name[(cond) ? 1 : -1]

DANS_STATIC_ASSERT(sizeof(dans_i8) == 1, i8_size);
DANS_STATIC_ASSERT(sizeof(dans_u8) == 1, u8_size);
DANS_STATIC_ASSERT(sizeof(dans_i16) == 2, i16_size);
DANS_STATIC_ASSERT(sizeof(dans_u16) == 2, u16_size);
DANS_STATIC_ASSERT(sizeof(dans_i32) == 4, i32_size);
DANS_STATIC_ASSERT(sizeof(dans_u32) == 4, u32_size);
DANS_STATIC_ASSERT(sizeof(dans_i64) == 8, i64_size);
DANS_STATIC_ASSERT(sizeof(dans_u64) == 8, u64_size);
DANS_STATIC_ASSERT(sizeof(dans_f32) == 4, f32_size);
DANS_STATIC_ASSERT(sizeof(dans_f64) == 8, f64_size);

#endif /* DANS_TYPES_H */

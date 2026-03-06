/* Minimal stdint.h for MSVC 2008 - VS2008 lacks stdint.h (added in VS2010).
 * Skips definitions if HALTools_Types.hpp already defined them (HALTOOLS_TYPES_HPP).
 */
#ifndef DCP06_MSVC2008_STDINT_H_
#define DCP06_MSVC2008_STDINT_H_

#include <limits.h>

#if _MSC_VER >= 1600
/* VS2010+ has stdint.h */
#include <cstdint>
#elif defined(HALTOOLS_TYPES_HPP)
/* HALTools_Types.hpp already defined int8_t etc. for VS2008 - avoid redefinition */
#else

#ifndef _W64
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif

/* Exact-width types - only define if not already defined */
#ifndef __int8_t_defined
#define __int8_t_defined
#if (_MSC_VER >= 1300)
typedef __int8            int8_t;
typedef __int16           int16_t;
typedef __int32           int32_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
#else
typedef signed char       int8_t;
typedef signed short      int16_t;
typedef signed int        int32_t;
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
#endif
typedef __int64           int64_t;
typedef unsigned __int64  uint64_t;
#endif

/* Least and fast types */
#ifndef __int_least8_t_defined
#define __int_least8_t_defined
typedef int8_t   int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef int8_t   int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;
typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;
#endif

/* Pointer types */
#ifdef _WIN64
typedef __int64    intptr_t;
typedef unsigned __int64 uintptr_t;
#else
typedef _W64 signed int   intptr_t;
typedef _W64 unsigned int uintptr_t;
#endif

/* Limit macros */
#define INT8_MIN   (-127 - 1)
#define INT16_MIN  (-32767 - 1)
#define INT32_MIN  (-2147483647 - 1)
#define INT64_MIN  ((int64_t)(-9223372036854775807LL - 1))

#define INT8_MAX   127
#define INT16_MAX  32767
#define INT32_MAX  2147483647
#define INT64_MAX  ((int64_t)9223372036854775807LL)

#define UINT8_MAX  0xff
#define UINT16_MAX 0xffff
#define UINT32_MAX 0xffffffff
#define UINT64_MAX ((uint64_t)0xffffffffffffffffULL)

#endif /* #else of _MSC_VER>=1600 / HALTOOLS_TYPES_HPP */
#endif /* DCP06_MSVC2008_STDINT_H_ */

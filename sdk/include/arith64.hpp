// GCC 32/64-bit integer arithmetic support for 32-bit systems that can't link
// to libgcc.

// Function prototypes and descriptions are taken from
// https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html.

// This file may be #include'd by another file, so we try not to pollute the
// namespace and we don't import any headers.

// All functions must be resolvable by the linker and therefore can't be inline
// or static, even if they're #included into the file where they'll be used.

// For best performance we try to avoid branching. This makes the code a little
// weird in places.

// See https://github.com/glitchub/arith64 for more information.
// This software is released as-is into the public domain, as described at
// https://unlicense.org. Do whatever you like with it.

#pragma once

extern "C"
{
    #define arith64_u64 unsigned long long int
    #define arith64_s64 signed long long int
    #define arith64_u32 unsigned int
    #define arith64_s32 int

    typedef union
    {
        arith64_u64 u64;
        arith64_s64 s64;
        struct
        {
    #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            arith64_u32 hi; arith64_u32 lo;
    #else
            arith64_u32 lo; arith64_u32 hi;
    #endif
        } u32;
        struct
        {
    #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            arith64_s32 hi; arith64_s32 lo;
    #else
            arith64_s32 lo; arith64_s32 hi;
    #endif
        } s32;
    } arith64_word;

    // extract hi and lo 32-bit words from 64-bit value
    #define arith64_hi(n) (arith64_word){.u64=n}.u32.hi
    #define arith64_lo(n) (arith64_word){.u64=n}.u32.lo

    // Negate a if b is negative, via invert and increment.
    #define arith64_neg(a, b) (((a) ^ ((((arith64_s64)(b)) >= 0) - 1)) + (((arith64_s64)(b)) < 0))
    #define arith64_abs(a) arith64_neg(a, a)

    arith64_s64 __absvdi2(arith64_s64 a);
    arith64_s64 __ashldi3(arith64_s64 a, int b);
    arith64_s64 __ashrdi3(arith64_s64 a, int b);
    int __clzsi2(arith64_u32 a);
    int __clzdi2(arith64_u64 a);
    int __ctzsi2(arith64_u32 a);
    int __ctzdi2(arith64_u64 a);
    arith64_u64 __divmoddi4(arith64_u64 a, arith64_u64 b, arith64_u64 *c);
    arith64_s64 __divdi3(arith64_s64 a, arith64_s64 b);
    int __ffsdi2(arith64_u64 a);
    arith64_u64 __lshrdi3(arith64_u64 a, int b);
    arith64_s64 __moddi3(arith64_s64 a, arith64_s64 b);
    int __popcountsi2(arith64_u32 a);
    int __popcountdi2(arith64_u64 a);
    arith64_u64 __udivdi3(arith64_u64 a, arith64_u64 b);
    arith64_u64 __umoddi3(arith64_u64 a, arith64_u64 b);
}
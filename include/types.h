/* Bringing Rust-like ergonomic types to C */
#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef bool b8;

typedef float f32;
typedef double f64;
typedef long double f128;

typedef long long isize;
typedef size_t usize;

#define null (void*) (0)

enum Result {
    Ok,
    Err
};
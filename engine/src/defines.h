//
// Created by jwraynor on 8/21/2023.
//

#pragma once

/**
 * Our string types
 */
typedef const char *conststr;
typedef char *str;

/**
 * Our pointer types
 */
typedef void *ptr;
typedef const void *constptr;

/**
 * Our unsigned integer types
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

/**
 * Our signed integer types
 */
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

/**
 * Our floating point types
 */
typedef float f32;
typedef double f64;

/**
 * Our boolean type
 */
typedef int b32;
typedef char b8;

#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif
/**
 * Check that our types are the correct size
 */
STATIC_ASSERT(sizeof(u8) == 1, "u8 is not 1 byte");
STATIC_ASSERT(sizeof(u16) == 2, "u16 is not 2 bytes");
STATIC_ASSERT(sizeof(u32) == 4, "u32 is not 4 bytes");
STATIC_ASSERT(sizeof(u64) == 8, "u64 is not 8 bytes");

STATIC_ASSERT(sizeof(i8) == 1, "i8 is not 1 byte");
STATIC_ASSERT(sizeof(i16) == 2, "i16 is not 2 bytes");
STATIC_ASSERT(sizeof(i32) == 4, "i32 is not 4 bytes");
STATIC_ASSERT(sizeof(i64) == 8, "i64 is not 8 bytes");

STATIC_ASSERT(sizeof(f32) == 4, "f32 is not 4 bytes");
STATIC_ASSERT(sizeof(f64) == 8, "f64 is not 8 bytes");

STATIC_ASSERT(sizeof(b32) == 4, "b32 is not 4 bytes");
STATIC_ASSERT(sizeof(b8) == 1, "b8 is not 1 byte");

/**
 * Our boolean values
 */
#define true 1
#define false 0

/**
 * Platform detection
 */

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define VPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64 bit windows is required"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
#define VPLATFORM_LINUX 1
#elif defined(__ANDROID__)
#define VPLATFORM_ANDROID 1
#elif defined(__APPLE__)
#define VPLATFORM_APPLE 1
#include <TagetConditionals.h>
#if TARGET_OS_IPHONE
#define VPLATFORM_IOS 1
#elif TARGET_OS_MAC
#define VPLATFORM_MAC 1
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform"
#endif

#ifdef PLATFORM_EXPORT
#if defined(VPLATFORM_WINDOWS)
#define VAPI __declspec(dllexport)
#elif defined(PLATFORM_LINUX)
#define VAPI __attribute__((visibility("default")))
#else
#define VAPI
#endif
#else
#if defined(VPLATFORM_WINDOWS)
#define VAPI __declspec(dllimport)
#else
#define VAPI
#endif
#endif
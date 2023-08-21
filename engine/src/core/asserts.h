//
// Created by jwraynor on 8/21/2023.
//

#pragma once

#include "defines.h"

#define VASSERTS_ENABLED

#ifdef VASSERTS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#elif
#define debugBreak() __builtin_trap()
#endif
#endif
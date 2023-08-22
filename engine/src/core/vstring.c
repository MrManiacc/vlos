//
// Created by jwraynor on 8/22/2023.
//
#include "core/vstring.h"
#include "core/mem.h"
#include <string.h>

u64 string_length(conststr string) {
    return strlen(string);
}

str string_duplicate(conststr string) {
    u64 length = string_length(string);
    str result = mem_alloc(length + 1, MEM_TAG_STRING);
    mem_copy(result, string, length);
    return result;
}

b8 strings_equal(conststr string1, conststr string2) {
    return strcmp(string1, string2) == 0;
}


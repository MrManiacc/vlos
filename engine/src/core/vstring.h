//
// Created by jwraynor on 8/22/2023.
//

#pragma once

#include "defines.h"
/**
 * @brief get the length of a string
 *
 * @param str the string to get the length of
 * @return u64 the length of the string
 */
u64 string_length(conststr str);

/**
 * @brief creates a new pointer to a string from the given string
 * @param str  the string to duplicate
 * @return  the duplicated string
 */
str string_duplicate(conststr str);

/**
 * @brief checks if the given string is equal to the other given string
 * @param str1 the first string
 * @param str2 the second string
 * @return b8 true if the strings are equal, false otherwise
 */
b8 strings_equal(conststr str1, conststr str2);



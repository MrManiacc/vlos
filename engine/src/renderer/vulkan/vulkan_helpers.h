//
// Created by jwraynor on 8/23/2023.
//

#pragma once

#include "vulkan_types.inl"

/**
 * Returns the string representation of the vulkan result
 * @param result the result to convert
 * @param extended if true, the extended version of the result is returned
 * @return the string representation of the result
 */
const char *vulkan_result_string(VkResult result, b8 extended);


/**
 * Checks iof the result is a success
 * @param result
 * @return
 */
b8 vulkan_result_is_success(VkResult result);


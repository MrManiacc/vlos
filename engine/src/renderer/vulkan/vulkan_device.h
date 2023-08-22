//
// Created by jwraynor on 8/22/2023.
//

#pragma once

#include "defines.h"
#include "renderer/vulkan/vulkan_types.inl"

/**
 * This should create our physical vulkan device
 * @param context the context to create the device for
 * @return whether or not the device was successfully created
 */
b8 vulkan_device_create(vulkan_context *context);

/**
 * Destroy our physical vulkan device. This should be called before the context is destroyed.
 * @param context the context to destroy the device for
 */
void vulkan_device_destroy(vulkan_context *context);
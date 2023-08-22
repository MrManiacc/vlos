//
// Created by jwraynor on 8/22/2023.
//

#pragma once

#include "defines.h"

/**
 * Creates a new surface for vulkan to render in to.
 * @param plat_state the platform state, specific to the platform
 * @param context  the vulkan context
 * @return true if the surface was created successfully, false otherwise
 */
b8 platform_create_vulkan_surface(struct platform_state *plat_state, struct vulkan_context *context);

/**
 * Collects all the required extensions for the vulkan instance
 * @param names_darray the names to populate
 */
void platform_required_extensions(const char ***names_darray);

/**
 * Created by jraynor on 8/22/2023.
 */
#pragma once

#include "vulkan_types.inl"

/**
 * Creates a renderpass that can be used to render to the screen. This renderpass is created with a single subpass.
 * @param context The vulkan context
 * @param out_renderpass The renderpass to create
 */
void vulkan_render_pass_create(
    vulkan_context *context,
    vulkan_render_pass *out_renderpass,
    f32 x, f32 y, f32 w, f32 h,
    f32 r, f32 g, f32 b, f32 a,
    f32 depth,
    u32 stencil
);

/**
* Destroys the given renderpass and frees all memory associated with it.
*/
void vulkan_render_pass_destroy(vulkan_context *context, vulkan_render_pass *renderpass);

/**
 * Begins the given renderpass. This will bind the renderpass to the given command buffer.
 * @param command_buffer The command buffer to bind the renderpass to
 * @param renderpass The renderpass to begin
 * @param framebuffer The framebuffer to bind to the renderpass
 */
void vulkan_render_pass_begin(vulkan_command_buffer *command_buffer,
                             vulkan_render_pass *renderpass,
                             VkFramebuffer framebuffer);

/**
* Ends the given renderpass. This will unbind the renderpass from the given command buffer.
 * @param command_buffer The command buffer to unbind the renderpass from
 * @param renderpass The renderpass to end
*/
void vulkan_render_pass_end(vulkan_command_buffer *command_buffer, vulkan_render_pass *renderpass);

//
// Created by James Raynor on 8/22/23.
//
#pragma once

#include "defines.h"

/**
 * The maximum length of a path
 */
#define MAX_PATH_LENGTH 512

/**
 * The maximum length of a file name
 */
#define MAX_FILE_NAME_LENGTH 256

/**
 * The maximum length of a file extension
 */
#define MAX_FILE_EXTENSION_LENGTH 32

typedef struct vfs_platform_state {
    /**
     * A function pointer to the platform specific file open function.
     * @param path The path to the file to open
     * @param mode The mode to open the file in (read, write, etc)
     * @return The file handle
     */
    void *(*open)(conststr path, conststr mode);
} vfs_platform_state;

/**
 * The internal context for the virtual file system. This is used to track the state of the file system.
 * This is per platform and should be called by the platform specific code.
 */
typedef struct vfs_context {
    void *internal_state;
} vfs_context;


/**
 * The type of the file
 */
typedef enum vfs_file_type {
    FILE_TYPE_UNKNOWN,
    FILE_TYPE_REGULAR,
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_SYMLINK,
    FILE_TYPE_PIPE,
    FILE_TYPE_SOCKET,
    FILE_TYPE_DEVICE,
    FILE_TYPE_MOUNT_POINT,
} file_type;
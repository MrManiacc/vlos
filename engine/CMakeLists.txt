
set(CMAKE_C_STANDARD 23)
# 1. Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
if (DEFINED VULKAN_SDK_PATH)
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
    set(Vulkan_FOUND "True")
else ()
    find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
    message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif ()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else ()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif ()

if (APPLE)
    # Glob's are evil, but this is a small project
    file(GLOB_RECURSE SOURCES "*.c" "*.h" "*.inl" "*.m")
else ()
    file(GLOB_RECURSE SOURCES "*.c" "*.h" "*.inl")
endif ()
add_library(engine SHARED ${SOURCES})
set_target_properties(engine PROPERTIES LINKER_LANGUAGE C)


if (APPLE)
    message("Building for macOS")
    # Allow for Objective-C compilation on macOS
    enable_language(OBJC OBJCXX)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(FOUNDATION_LIBRARY Foundation)
    mark_as_advanced(COCOA_LIBRARY FOUNDATION_LIBRARY)
    target_link_libraries(engine ${COCOA_LIBRARY} ${FOUNDATION_LIBRARY})
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Include Vulkan header files from Vulkan SDK
    include_directories(AFTER ${VULKAN_PATH}/Include)

    # Link directory for vulkan-1
    link_directories(${VULKAN_PATH}/Bin;${VULKAN_PATH}/Lib;)
endif ()


target_include_directories(engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
)
target_compile_definitions(engine PUBLIC -DPLATFORM_EXPORT)

target_link_libraries(engine
        ${Vulkan_LIBRARIES}
)

target_compile_definitions(engine PUBLIC -DDEBUG_BUILD)

cmake_minimum_required(VERSION 3.25)
project(vlos C)


##check if we are in debug build or release
#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#    # Set the cmake runtime output directory to our build folder
#    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/debug)
#else ()
#    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/release)
#endif ()


set(CMAKE_C_STANDARD 23)
add_subdirectory(engine)
add_subdirectory(sandbox)

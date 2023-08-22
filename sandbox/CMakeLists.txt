
set(CMAKE_C_STANDARD 23)


# Globs the resources relative to the source directory, inside the folder "src" recursively
file(GLOB_RECURSE CFILES "*.c")
file(GLOB_RECURSE HFILES "*.h")

add_executable(sandbox ${CFILES} ${HFILES})
set_target_properties(sandbox PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(sandbox PRIVATE engine)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

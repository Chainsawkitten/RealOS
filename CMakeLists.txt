cmake_minimum_required(VERSION 2.8.12)

project(RealOSProject)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# This function should be passed a list of all files in a target. It will automatically generate
# file groups following the directory hierarchy, so that the layout of the files in IDEs matches the
# one in the filesystem.
function(create_directory_groups)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    foreach(file_name ${ARGV})
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

# Source files
set(SRCS
        src/Block.cpp
        src/Directory.cpp
        src/File.cpp
        src/FileSystem.cpp
        src/main.cpp
        src/MemBlockDevice.cpp
        src/Shell.cpp
    )

# Header files
set(HEADERS
        src/Block.hpp
        src/Directory.hpp
        src/File.hpp
        src/FileSystem.hpp
        src/MemBlockDevice.hpp
        src/Shell.hpp
    )

# Target: Shell
create_directory_groups(${SRCS} ${HEADERS})
add_executable(Shell ${SRCS} ${HEADERS})
target_compile_features(Shell PRIVATE cxx_nullptr)

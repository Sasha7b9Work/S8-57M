cmake_minimum_required(VERSION 2.9)

# Set CMake policy required by UrhoCommon module
if (COMMAND cmake_policy)
    # Libraries linked via full path no longer produce linker search paths
    cmake_policy (SET CMP0003 NEW)

    # INTERFACE_LINK_LIBRARIES defines the link interface
    cmake_policy (SET CMP0022 NEW)

    # MACOSX_RPATH is enabled by default
    cmake_policy (SET CMP0042 NEW)

    # MSVC runtime library flags are selected by an abstraction.
    cmake_policy (SET CMP0091 NEW)
endif ()

# Set project name
project (S8-57_GUI)

set (TARGET_NAME S8-57_GUI)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(Device)

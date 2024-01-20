
cmake_minimum_required(VERSION 3.5)
project(libquack)

#-------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#-------------------------------------------------------------------------------

include(GNUInstallDirs)

#-------------------------------------------------------------------------------

add_library(libquack
    src/base.c)

add_library(libquack::libquack ALIAS libquack)

target_include_directories(libquack
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(libquack
    PROPERTIES
        PREFIX ""
        C_STANDARD 99
        C_EXTENSIONS OFF
        C_VISIBILITY_PRESET hidden)

target_compile_definitions(libquack PRIVATE QU_BUILD)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(libquack PRIVATE QU_SHARED)
endif()

if(MSVC)
    target_compile_definitions(libquack PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(libquack PRIVATE /W4)
else()
    target_compile_options(libquack PRIVATE -Wall -Wextra)
endif()



cmake_minimum_required(VERSION 3.5)
project(libquack)

#-------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(QU_BUILD_EXAMPLES "Build examples" ON)
option(QU_USE_X11 "Use X11" OFF)

#-------------------------------------------------------------------------------

include(GNUInstallDirs)

#-------------------------------------------------------------------------------

add_library(libquack
    src/base.c
    src/core.c
    src/core_null.c
    src/core_x11.c
    src/graphics.c
    src/graphics_null.c
    src/log.c
    src/platform_posix.c)

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

if(QU_USE_X11)
    find_package(X11 REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_X11)
    target_include_directories(libquack PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(libquack PRIVATE ${X11_LIBRARIES})
endif()

#-------------------------------------------------------------------------------

if(QU_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


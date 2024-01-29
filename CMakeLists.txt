
cmake_minimum_required(VERSION 3.5)
project(libquack)

#-------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(QU_BUILD_EXAMPLES "Build examples" ON)
option(QU_USE_OPENGL "Use OpenGL" OFF)
option(QU_USE_X11 "Use X11" OFF)
option(QU_USE_OPENAL "Use OpenAL" OFF)
option(QU_USE_VORBIS "Use Vorbis" OFF)

#-------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

#-------------------------------------------------------------------------------

add_library(libquack
    src/algebra.c
    src/audio.c
    src/audio_null.c
    src/audio_openal.c
    src/base.c
    src/core.c
    src/core_null.c
    src/core_x11.c
    src/fs.c
    src/graphics.c
    src/graphics_gl3.c
    src/graphics_null.c
    src/handle.c
    src/log.c
    src/platform_posix.c
    src/util.c)

add_library(libquack::libquack ALIAS libquack)

target_include_directories(libquack
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/third-party)

set_target_properties(libquack
    PROPERTIES
        PREFIX ""
        C_STANDARD 99
        C_EXTENSIONS ON
        C_VISIBILITY_PRESET hidden)

target_compile_definitions(libquack PRIVATE QU_BUILD)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(libquack PRIVATE QU_SHARED)
endif()

if(MSVC)
    target_compile_definitions(libquack PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(libquack PRIVATE /W4)
else()
    target_compile_options(libquack PRIVATE -Wall -Wextra
        -Wno-type-limits -Wno-unused-parameter)
endif()

find_library(MATH_LIBRARY m)

if(MATH_LIBRARY)
    target_link_libraries(libquack PRIVATE ${MATH_LIBRARY})
endif()

if(QU_USE_OPENGL)
    find_package(OpenGL REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_OPENGL)
    target_include_directories(libquack PRIVATE ${OPENGL_INCLUDE_DIR})
    target_link_libraries(libquack PRIVATE ${OPENGL_gl_LIBRARY})
endif()

if(QU_USE_X11)
    find_package(X11 REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_X11)
    target_include_directories(libquack PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(libquack PRIVATE ${X11_LIBRARIES})
endif()

if(QU_USE_OPENAL)
    find_package(OpenAL REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_OPENAL)
    target_include_directories(libquack PRIVATE ${OPENAL_INCLUDE_DIR})
    target_link_libraries(libquack PRIVATE ${OPENAL_LIBRARY})
endif()

if(QU_USE_VORBIS)
    find_package(Vorbis REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_VORBIS)
    target_link_libraries(libquack PRIVATE Vorbis::vorbisfile)
endif()

#-------------------------------------------------------------------------------

if(QU_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

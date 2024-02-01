
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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

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
        C_VISIBILITY_PRESET hidden
        DEFINE_SYMBOL QU_BUILD)

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

#-------------------------------------------------------------------------------
# Dependencies

set(_QU_PC_REQUIRES_PRIVATE "")
set(_QU_PC_LIBS_PRIVATE "")

find_library(MATH_LIBRARY m)

if(MATH_LIBRARY)
    target_link_libraries(libquack PUBLIC ${MATH_LIBRARY})
    list(APPEND _QU_PC_LIBS_PRIVATE "-lm")
endif()

if(QU_USE_OPENGL)
    if(POLICY CMP0072)
        cmake_policy(SET CMP0072 NEW)
    endif()

    find_package(OpenGL REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_OPENGL)

    if(TARGET OpenGL::GL)
        target_link_libraries(libquack PRIVATE OpenGL::GL)
    else()
        target_include_directories(libquack PRIVATE ${OPENGL_INCLUDE_DIR})
        target_link_libraries(libquack PRIVATE ${OPENGL_gl_LIBRARY})
    endif()

    list(APPEND _QU_PC_REQUIRES_PRIVATE "opengl")
endif()

if(QU_USE_X11)
    find_package(X11 REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_X11)
    target_include_directories(libquack PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(libquack PRIVATE ${X11_LIBRARIES})

    list(APPEND _QU_PC_REQUIRES_PRIVATE "x11")
endif()

if(QU_USE_OPENAL)
    find_package(OpenAL REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_OPENAL)
    target_include_directories(libquack PRIVATE ${OPENAL_INCLUDE_DIR})
    target_link_libraries(libquack PRIVATE ${OPENAL_LIBRARY})

    list(APPEND _QU_PC_REQUIRES_PRIVATE "openal")
endif()

if(QU_USE_VORBIS)
    find_package(Vorbis REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_VORBIS)
    target_link_libraries(libquack PRIVATE Vorbis::vorbisfile)

    list(APPEND _QU_PC_REQUIRES_PRIVATE "vorbisfile")
endif()

#-------------------------------------------------------------------------------
# Set installation paths

set(QU_INCLUDE_PATH "${CMAKE_INSTALL_INCLUDEDIR}"
    CACHE PATH "Location of libquack header files")

set(QU_LIBRARY_PATH "${CMAKE_INSTALL_LIBDIR}"
    CACHE PATH "Location of libquack library files")

set(QU_RUNTIME_PATH "${CMAKE_INSTALL_BINDIR}"
    CACHE PATH "Location of libquack runtime files")

set(QU_CONFIG_PATH "${CMAKE_INSTALL_LIBDIR}/cmake/libquack"
    CACHE PATH "Location of CMake configuration files")

#-------------------------------------------------------------------------------
# Generate CMake config files

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/libquack-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libquack-config.cmake"
    INSTALL_DESTINATION "${QU_CONFIG_PATH}")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/libquack-config-version.cmake"
    VERSION 0.1.0
    COMPATIBILITY SameMajorVersion)

#-------------------------------------------------------------------------------
# Generate .pc (pkg-config) file

set(QU_PC_LIBS "-lquack")
string(REPLACE ";" " " QU_PC_REQUIRES_PRIVATE "${_QU_PC_REQUIRES_PRIVATE}")
string(REPLACE ";" " " QU_PC_LIBS_PRIVATE "${_QU_PC_LIBS_PRIVATE}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/libquack.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libquack.pc"
    @ONLY)

#-------------------------------------------------------------------------------
# Examples

if(QU_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

#-------------------------------------------------------------------------------
# Installation

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/libqu"
    DESTINATION ${QU_INCLUDE_PATH})

install(TARGETS libquack
    EXPORT libquack-targets
    ARCHIVE DESTINATION ${QU_LIBRARY_PATH}
    LIBRARY DESTINATION ${QU_LIBRARY_PATH}
    RUNTIME DESTINATION ${QU_RUNTIME_PATH})

install(EXPORT libquack-targets
    NAMESPACE libquack::
    FILE libquack-targets.cmake
    DESTINATION ${QU_CONFIG_PATH})

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libquack-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libquack-config-version.cmake"
    DESTINATION ${QU_CONFIG_PATH})

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/FindOgg.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/FindVorbis.cmake"
    DESTINATION "${QU_CONFIG_PATH}/modules")

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libquack.pc"
    DESTINATION "${QU_LIBRARY_PATH}/pkgconfig")

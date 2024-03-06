
cmake_minimum_required(VERSION 3.5)

#-------------------------------------------------------------------------------

set(QU_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/libquack/libquack.h")

set(QU_VERSION_MAJOR_REGEX "#define[ \t]+QU_VERSION_MAJOR[ \t]+([0-9]+)")
set(QU_VERSION_MINOR_REGEX "#define[ \t]+QU_VERSION_MINOR[ \t]+([0-9]+)")
set(QU_VERSION_PATCH_REGEX "#define[ \t]+QU_VERSION_PATCH[ \t]+([0-9]+)")

file(STRINGS ${QU_HEADER} QU_VERSION_MAJOR REGEX ${QU_VERSION_MAJOR_REGEX})
file(STRINGS ${QU_HEADER} QU_VERSION_MINOR REGEX ${QU_VERSION_MINOR_REGEX})
file(STRINGS ${QU_HEADER} QU_VERSION_PATCH REGEX ${QU_VERSION_PATCH_REGEX})

string(REGEX REPLACE ${QU_VERSION_MAJOR_REGEX} "\\1" QU_VERSION_MAJOR ${QU_VERSION_MAJOR})
string(REGEX REPLACE ${QU_VERSION_MINOR_REGEX} "\\1" QU_VERSION_MINOR ${QU_VERSION_MINOR})
string(REGEX REPLACE ${QU_VERSION_PATCH_REGEX} "\\1" QU_VERSION_PATCH ${QU_VERSION_PATCH})

#-------------------------------------------------------------------------------

project(libquack
    VERSION "${QU_VERSION_MAJOR}.${QU_VERSION_MINOR}.${QU_VERSION_PATCH}")

#-------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

#-------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(QU_BUILD_TESTS "Build tests" ON)
option(QU_USE_OPENGL "Use OpenGL" ON)
cmake_dependent_option(QU_USE_X11 "Use X11" ON "UNIX;NOT APPLE" OFF)
option(QU_USE_OPENAL "Use OpenAL" ON)
option(QU_USE_VORBIS "Use Vorbis" ON)

#-------------------------------------------------------------------------------

add_library(libquack
    src/algebra.c
    src/audio.c
    src/audio_null.c
    src/audio_openal.c
    src/base.c
    src/core.c
    src/core_null.c
    src/core_win32.c
    src/core_x11.c
    src/fs.c
    src/graphics.c
    src/graphics_gl3.c
    src/graphics_null.c
    src/handle.c
    src/log.c
    src/platform_posix.c
    src/platform_win32.c
    src/util.c)

add_library(libquack::libquack ALIAS libquack)

target_include_directories(libquack
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/libquack>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libquack>
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
    target_compile_options(libquack PRIVATE /W4 /wd4100 /wd4152 /wd4206)
else()
    target_compile_options(libquack PRIVATE -Wall -Wextra
        -Wno-type-limits -Wno-unused-parameter)
endif()

#-------------------------------------------------------------------------------
# Dependencies

set(_QU_PC_LIBS "-lquack")
set(_QU_PC_LIBS_PRIVATE "")
set(_QU_PC_REQUIRES_PRIVATE "")

find_package(Threads REQUIRED)
target_link_libraries(libquack PUBLIC Threads::Threads)

if(CMAKE_THREAD_LIBS_INIT)
    list(APPEND _QU_PC_LIBS_PRIVATE "-l${CMAKE_THREAD_LIBS_INIT}")
endif()

find_library(MATH_LIBRARY m)

if(MATH_LIBRARY)
    target_link_libraries(libquack PUBLIC ${MATH_LIBRARY})
    list(APPEND _QU_PC_LIBS_PRIVATE "-lm")
endif()

if(CMAKE_DL_LIBS)
    target_link_libraries(libquack PUBLIC ${CMAKE_DL_LIBS})
    list(APPEND _QU_PC_LIBS_PRIVATE "-l${CMAKE_DL_LIBS}")
endif()

if(QU_USE_OPENGL)
    if(POLICY CMP0072)
        cmake_policy(SET CMP0072 NEW)
    endif()

    find_package(OpenGL REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_OPENGL)
    target_include_directories(libquack PRIVATE ${OPENGL_INCLUDE_DIR})
endif()

if(QU_USE_X11)
    find_package(X11 REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_X11)
    target_include_directories(libquack PRIVATE ${X11_INCLUDE_DIR})
endif()

if(QU_USE_OPENAL)
    target_compile_definitions(libquack PRIVATE QU_USE_OPENAL)
    target_include_directories(libquack PRIVATE "third-party/al")
endif()

if(QU_USE_VORBIS)
    find_package(Vorbis REQUIRED)
    target_compile_definitions(libquack PRIVATE QU_USE_VORBIS)
    target_link_libraries(libquack PRIVATE Vorbis::vorbisfile)
    list(APPEND _QU_PC_REQUIRES_PRIVATE "vorbisfile")
endif()

if(WIN32)
    target_link_libraries(libquack PRIVATE shlwapi)
    list(APPEND _QU_PC_LIBS_PRIVATE "-lshlwapi")
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

set(QU_TEST_PATH "${CMAKE_INSTALL_LIBEXECDIR}/libquack"
    CACHE PATH "Location of libquack test executable files")

#-------------------------------------------------------------------------------
# Generate CMake config files

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/libquack-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libquack-config.cmake"
    INSTALL_DESTINATION "${QU_CONFIG_PATH}")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/libquack-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

#-------------------------------------------------------------------------------
# Generate .pc (pkg-config) file

string(REPLACE ";" " " QU_PC_LIBS "${_QU_PC_LIBS}")
string(REPLACE ";" " " QU_PC_LIBS_PRIVATE "${_QU_PC_LIBS_PRIVATE}")
string(REPLACE ";" " " QU_PC_REQUIRES_PRIVATE "${_QU_PC_REQUIRES_PRIVATE}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/libquack.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libquack.pc"
    @ONLY)

#-------------------------------------------------------------------------------
# Tests

if(QU_BUILD_TESTS)
    add_subdirectory(tests)
endif()

#-------------------------------------------------------------------------------
# Installation

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/libquack"
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
